#include "WallGrid.h"
#include "Block.h"
#include <cmath>
#include <set>

void WallGrid::Build(const std::vector<Block*>& walls, float cellSize) {
    m_cellSize = cellSize;
    m_cells.clear();

    for (Block* b : walls) {
        sf::FloatRect bounds = b->rect.getGlobalBounds();
        int minCx = static_cast<int>(std::floor(bounds.position.x / m_cellSize));
        int maxCx = static_cast<int>(std::floor((bounds.position.x + bounds.size.x) / m_cellSize));
        int minCy = static_cast<int>(std::floor(bounds.position.y / m_cellSize));
        int maxCy = static_cast<int>(std::floor((bounds.position.y + bounds.size.y) / m_cellSize));

        for (int cx = minCx; cx <= maxCx; ++cx)
            for (int cy = minCy; cy <= maxCy; ++cy)
                m_cells[Key(cx, cy)].push_back(b);
    }
}

std::vector<Block*> WallGrid::QueryNear(sf::Vector2f pos, float radius) const {
    std::vector<Block*> result;
    std::set<Block*> seen; // évite les doublons (un mur peut couvrir plusieurs cellules)

    int minCx = static_cast<int>(std::floor((pos.x - radius) / m_cellSize));
    int maxCx = static_cast<int>(std::floor((pos.x + radius) / m_cellSize));
    int minCy = static_cast<int>(std::floor((pos.y - radius) / m_cellSize));
    int maxCy = static_cast<int>(std::floor((pos.y + radius) / m_cellSize));

    for (int cx = minCx; cx <= maxCx; ++cx) {
        for (int cy = minCy; cy <= maxCy; ++cy) {
            auto it = m_cells.find(Key(cx, cy));
            if (it == m_cells.end()) continue;
            for (Block* b : it->second) {
                if (seen.insert(b).second) result.push_back(b);
            }
        }
    }
    return result;
}

std::vector<Block*> WallGrid::QueryAlongSegment(sf::Vector2f start, sf::Vector2f end) const {
    // Englobe le segment dans une zone de recherche simple
    sf::Vector2f mid = (start + end) / 2.f;
    float dist = std::hypot(end.x - start.x, end.y - start.y);
    return QueryNear(mid, dist / 2.f + m_cellSize);
}