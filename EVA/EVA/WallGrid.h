#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

class Block;

class WallGrid {
public:
    void Build(const std::vector<Block*>& walls, float cellSize = 64.f);
    std::vector<Block*> QueryNear(sf::Vector2f pos, float radius) const;
    std::vector<Block*> QueryAlongSegment(sf::Vector2f start, sf::Vector2f end) const;

private:
    float m_cellSize = 64.f;
    std::unordered_map<long long, std::vector<Block*>> m_cells;

    long long Key(int cx, int cy) const { return (static_cast<long long>(cx) << 32) ^ (unsigned int)cy; }
};