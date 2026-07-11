#pragma once
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "Zone.h"

class Soldat;

class ZoneManager {
public:
    void ExtractZonesFromFile(const std::string& filepath,
        const std::vector<std::pair<char, std::string>>& capturableSymbols,
        float tileSize,
        sf::Vector2f levelOffset = { 0.f, 0.f });
    void ExtractWaypointsFromFile(const std::string& filepath,
        float tileSize,
        sf::Vector2f levelOffset = { 0.f, 0.f });
    void Update(float dt, std::vector<Soldat*>& soldats);
    void ResetZones();
    std::vector<Zone>& GetZones() { return m_zones; }
    const std::map<int, sf::Vector2f>& GetWaypoints() const { return m_waypoints; }
    bool GetWaypointById(int id, sf::Vector2f& outPos) const;

    // Renvoie l'ID d'un waypoint accessible depuis currentWaypointId, en excluant
    // ceux occupés par un autre soldat. -1 = pas de waypoint courant (premier choix libre).
    int GetRandomWaypointId(int currentWaypointId, sf::Vector2f fromPos) const;
    sf::Vector2f GetRandomWaypoint() const; // conservé pour compat, prend id -1

    void UpdateOccupiedWaypoints(const std::vector<Soldat*>& soldats, float occupyRadius = 40.f);
    Zone* GetZoneOwnedByTeam(Team team);
    Zone* GetZoneBySymbol(char symbol);
    Zone* GetNearestContestableZone(sf::Vector2f from, Team team);
    sf::Vector2f GetNextWaypoint(sf::Vector2f from, sf::Vector2f target, float acceptRadius = 30.f) const;

private:
    std::vector<Zone> m_zones;
    std::map<int, sf::Vector2f> m_waypoints;
    std::vector<int> m_occupiedWaypointIds;

    // Graphe d'adjacence : depuis quel(s) id(s) peut-on aller vers quel(s) id(s)
    static const std::map<int, std::vector<int>> kWaypointGraph;
};