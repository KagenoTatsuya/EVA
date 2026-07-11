#include "ZoneManager.h"
#include "Battle.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <random>

void ZoneManager::ExtractZonesFromFile(const std::string& filepath, const std::vector<std::pair<char, std::string>>& capturableSymbols, float tileSize, sf::Vector2f levelOffset) {
    m_zones.clear();

    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::vector<std::string> grid;
    std::string line;
    int lineNumber = -1;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) { firstLine = false; continue; } // ignore l'en-tête (nom du niveau)
        if (line == "Next Level") {
            std::getline(file, line);
            continue;
        }
        if (line == "Previous Level") {
            std::getline(file, line);
            continue;
        }
        lineNumber++;
        grid.push_back(line);
    }

    for (const auto& [symbol, name] : capturableSymbols) {
        int minRow = std::numeric_limits<int>::max(), maxRow = std::numeric_limits<int>::min();
        int minCol = std::numeric_limits<int>::max(), maxCol = std::numeric_limits<int>::min();
        bool found = false;

        for (int row = 0; row < (int)grid.size(); ++row) {
            for (int col = 0; col < (int)grid[row].size(); ++col) {
                if (grid[row][col] == symbol) {
                    found = true;
                    minRow = std::min(minRow, row);
                    maxRow = std::max(maxRow, row);
                    minCol = std::min(minCol, col);
                    maxCol = std::max(maxCol, col);
                }
            }
        }
        if (!found) continue;

        Zone z;
        z.symbol = symbol;
        z.displayName = name;
        z.bounds = sf::FloatRect(
            { levelOffset.x + minCol * tileSize, levelOffset.y + minRow * tileSize },
            { (maxCol - minCol + 1) * tileSize, (maxRow - minRow + 1) * tileSize }
        );
        m_zones.push_back(z);
    }
}

void ZoneManager::ExtractWaypointsFromFile(const std::string& filepath,
    float tileSize,
    sf::Vector2f levelOffset)
{
    m_waypoints.clear();

    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = -1;

    while (std::getline(file, line)) {
        if (line == "Next Level") { std::getline(file, line); continue; }
        if (line == "Previous Level") { std::getline(file, line); continue; }
        lineNumber++;

        for (int col = 0; col < (int)line.size(); ++col) {
            char c = line[col];
            if (std::isdigit(static_cast<unsigned char>(c))) {
                int id = c - '0'; // '0' -> 0, '1' -> 1, ..., '9' -> 9

                sf::Vector2f pos(
                    levelOffset.x + col * tileSize + tileSize / 2.f,
                    levelOffset.y + lineNumber * tileSize + tileSize / 2.f
                );

                m_waypoints[id] = pos; // écrase si le chiffre apparaît plusieurs fois
            }
        }
    }
}

namespace {
    std::map<int, std::vector<int>> BuildSymmetricWaypointGraph() {
        // Chaque paire = un trajet autorisé dans les deux sens
        static const std::vector<std::pair<int, int>> edges = {
            { 1, 4 },
            { 2, 4 },
            { 5, 4 },
            { 5, 3 },
            { 6, 4 },
            { 6, 3 },
            { 9, 3 },
            { 8, 3 },
        };

        std::map<int, std::vector<int>> graph;
        for (const auto& [a, b] : edges) {
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
        return graph;
    }
}

const std::map<int, std::vector<int>> ZoneManager::kWaypointGraph = BuildSymmetricWaypointGraph();

void ZoneManager::UpdateOccupiedWaypoints(const std::vector<Soldat*>& soldats, float occupyRadius) {
    m_occupiedWaypointIds.clear();
    const float r2 = occupyRadius * occupyRadius;

    for (const auto& [id, wpPos] : m_waypoints) {
        for (Soldat* s : soldats) {
            if (!s->alive) continue;
            sf::Vector2f d = s->rect.getPosition() - wpPos;
            if (d.x * d.x + d.y * d.y <= r2) {
                m_occupiedWaypointIds.push_back(id);
                break;
            }
        }
    }
}

int ZoneManager::GetRandomWaypointId(int currentWaypointId, sf::Vector2f fromPos) const {
    static std::mt19937 rng(std::random_device{}());

    if (m_waypoints.empty()) return -1;

    std::vector<int> candidates;
    auto it = kWaypointGraph.find(currentWaypointId);

    if (currentWaypointId == -1 || it == kWaypointGraph.end()) {
        // Premier choix : on prend le waypoint le PLUS PROCHE du soldat
        // (pas de pathfinding -> il ne faut pas envoyer le soldat traverser toute la carte)
        int nearestId = -1;
        float bestDist = std::numeric_limits<float>::max();
        for (const auto& [id, pos] : m_waypoints) {
            float dx = pos.x - fromPos.x, dy = pos.y - fromPos.y;
            float d2 = dx * dx + dy * dy;
            if (d2 < bestDist) { bestDist = d2; nearestId = id; }
        }
        if (nearestId != -1) candidates = { nearestId };
    }
    else {
        candidates = it->second;
    }

    std::vector<int> available;
    for (int id : candidates) {
        bool occupied = std::find(m_occupiedWaypointIds.begin(), m_occupiedWaypointIds.end(), id)
            != m_occupiedWaypointIds.end();
        if (!occupied) available.push_back(id);
    }

    const std::vector<int>* pool = available.empty() ? &candidates : &available;
    if (pool->empty()) return -1;

    std::uniform_int_distribution<size_t> dist(0, pool->size() - 1);
    return (*pool)[dist(rng)];
}

sf::Vector2f ZoneManager::GetRandomWaypoint() const {
    int id = GetRandomWaypointId(-1, { 0.f, 0.f }); // conservé pour compat, non utilisé par les soldats
    if (id == -1) return { 0.f, 0.f };
    return m_waypoints.at(id);
}
bool ZoneManager::GetWaypointById(int id, sf::Vector2f& outPos) const {
    auto it = m_waypoints.find(id);
    if (it == m_waypoints.end()) return false;
    outPos = it->second;
    return true;
}

sf::Vector2f ZoneManager::GetNextWaypoint(sf::Vector2f from, sf::Vector2f target, float acceptRadius) const {
    float directDist = std::hypot(target.x - from.x, target.y - from.y);

    sf::Vector2f best = target;
    float bestScore = directDist;

    for (const auto& [id, wp] : m_waypoints) {
        float distFromMe = std::hypot(wp.x - from.x, wp.y - from.y);
        if (distFromMe < acceptRadius) continue;

        float distWpToTarget = std::hypot(target.x - wp.x, target.y - wp.y);
        if (distWpToTarget >= directDist) continue;

        float score = distFromMe + distWpToTarget;
        if (score < bestScore) {
            bestScore = score;
            best = wp;
        }
    }

    return best;
}

void ZoneManager::ResetZones() {
    for (Zone& z : m_zones) {
        z.captureProgress = 0.f;
        z.owner = ZoneOwner::Neutral;
    }
}

void ZoneManager::Update(float dt, std::vector<Soldat*>& soldats) {
    UpdateOccupiedWaypoints(soldats); // recalcule les waypoints occupés cette frame
    for (Zone& zone : m_zones) {
        int orangeCount = 0, bleuCount = 0;

        for (Soldat* s : soldats) {
            if (!s->alive) continue;
            if (zone.bounds.contains(s->rect.getPosition())) {
                (s->GetTeam() == Team::Orange ? orangeCount : bleuCount)++;
            }
        }

        if (orangeCount > bleuCount)
            zone.captureProgress -= Zone::kCaptureSpeed * dt * (orangeCount - bleuCount);
        else if (bleuCount > orangeCount)
            zone.captureProgress += Zone::kCaptureSpeed * dt * (bleuCount - orangeCount);
        // Égalité = zone contestée, elle ne bouge pas

        zone.captureProgress = std::clamp(zone.captureProgress, -Zone::kCaptureMax, Zone::kCaptureMax);

        if (zone.captureProgress <= -Zone::kCaptureMax) zone.owner = ZoneOwner::Orange;
        else if (zone.captureProgress >= Zone::kCaptureMax) zone.owner = ZoneOwner::Bleu;
        else if (zone.captureProgress == 0.f) zone.owner = ZoneOwner::Neutral;
    }
}

Zone* ZoneManager::GetNearestContestableZone(sf::Vector2f from, Team team) {
    Zone* best = nullptr;
    float bestDist = std::numeric_limits<float>::max();
    ZoneOwner myOwner = (team == Team::Orange) ? ZoneOwner::Orange : ZoneOwner::Bleu;

    for (Zone& zone : m_zones) {
        if (zone.owner == myOwner) continue; // déjà à nous
        sf::Vector2f c = zone.GetCenter();
        float dx = c.x - from.x, dy = c.y - from.y;
        float dist = dx * dx + dy * dy;
        if (dist < bestDist) { bestDist = dist; best = &zone; }
    }
    return best;
}

Zone* ZoneManager::GetZoneOwnedByTeam(Team team) {
    ZoneOwner myOwner = (team == Team::Orange) ? ZoneOwner::Orange : ZoneOwner::Bleu;
    for (Zone& zone : m_zones) {
        if (zone.owner == myOwner) return &zone;
    }
    return nullptr;
}

Zone* ZoneManager::GetZoneBySymbol(char symbol) {
    for (Zone& zone : m_zones) {
        if (zone.symbol == symbol) return &zone;
    }
    return nullptr;
}