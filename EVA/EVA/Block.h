#pragma once

#include <SFML/Graphics.hpp>

class Block {
protected:
	float width;
	float height;
	std::string blockType;
	std::string PreviousLevel;
	std::string currentLevel;
	std::string nextLevel;
	sf::Texture* texture;
	sf::Sprite* sprite;
public:
	sf::RectangleShape rect;
	bool isNextLevel;
	bool isPrevLevel;
	bool isUse;
public:
public:
	Block();
	virtual ~Block() {};
	void Render(sf::RenderTarget& target);

	inline float GetPosX() { return rect.getPosition().x; }
	inline float GetRightX() { return rect.getPosition().x + width; }
	inline float GetPosY() { return rect.getPosition().y; }
	inline float GetBottomY() { return rect.getPosition().y + height; }
	inline float GetWidth() { return width; }
	inline float GetHeight() { return height; }
	inline std::string GetBlockType() { return blockType; }
};

class MBlock : public Block {
public:
	MBlock(float posx, float posy);
	~MBlock() override {};
};

class EndBlock : public Block {
public:
	EndBlock(float posx, float posy);
	~EndBlock() override {};
};

class Item : public Block {
public:
	Item(float posx, float posy);
	~Item() override {};
};