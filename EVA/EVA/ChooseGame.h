#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>*

class ChooseGame {
public:

	sf::Texture* image;
	sf::Sprite* sprite;
	float screenW, screenH;

	ChooseGame();
	ChooseGame(float screenW, float screenH);

	void Render(sf::RenderWindow& window);

	~ChooseGame();


};

