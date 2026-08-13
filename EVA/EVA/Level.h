#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Block.h"

struct Texture;
struct Sprite;
class Block;

class Level {

	std::vector<Block*> blocks;
	std::string prevLevel;
	std::string nextLevel;
	float mapWidth = 0.f;
	float mapHeight = 0.f;

public:
	bool isLoad;
	float GetMapWidth() const { return mapWidth; }
	float GetMapHeight() const { return mapHeight; }

	Level();
	Level(std::string path1);
	~Level();
	void SetOffset(float x, float y);
	void Render(sf::RenderTarget& target);
	void Update(int currentLvl, int& newLvl, float dt, float now, float playerX, float playerY, float playerW, bool& onHZone);

	inline std::vector<Block*>& GetBlocks() { return blocks; }
	inline std::string GetPrevLevel() { return prevLevel; }
	inline std::string GetNextLevel() { return nextLevel; }
	inline bool GetLoadState() { return isLoad; }
};
