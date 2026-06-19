#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

class ChooseGame {
public:

	ChooseGame();

	void Render(sf::RenderWindow& window);

	~ChooseGame();

private:
	sf::Texture* image;
	sf::Sprite* sprite;
};

