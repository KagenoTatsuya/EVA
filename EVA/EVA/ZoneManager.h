#pragma once
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "Zone.h"

class Soldat;
class Entity;

class ZoneManager {
public:
    void ExtractZonesFromFile(const std::string& filepath,
        const std::vector<std::pair<char, std::string>>& capturableSymbols,
        float tileSize,
        sf::Vector2f levelOffset = { 0.f, 0.f });
    void ExtractWaypointsFromFile(const std::string& filepath,
        float tileSize,
        sf::Vector2f levelOffset = { 0.f, 0.f });
    void Update(float dt, std::vector<Soldat*>& soldats, Entity* player);
    void ResetZones();
    std::vector<Zone>& GetZones() { return m_zones; }
    const std::map<int, sf::Vector2f>& GetWaypoints() const { return m_waypoints; }
    bool GetWaypointById(int id, sf::Vector2f& outPos) const;
    int GetRandomWaypointId(int currentWaypointId, sf::Vector2f fromPos) const;
    sf::Vector2f GetRandomWaypoint() const;
    void UpdateOccupiedWaypoints(const std::vector<Soldat*>& soldats, float occupyRadius = 40.f);
    Zone* GetZoneOwnedByTeam(Team team);
    Zone* GetZoneBySymbol(char symbol);
    Zone* GetNearestContestableZone(sf::Vector2f from, Team team);
    sf::Vector2f GetNextWaypoint(sf::Vector2f from, sf::Vector2f target, float acceptRadius = 30.f) const;

private:
    std::vector<Zone> m_zones;
    std::map<int, sf::Vector2f> m_waypoints;
    std::vector<int> m_occupiedWaypointIds;
    static const std::map<int, std::vector<int>> kWaypointGraph;
};