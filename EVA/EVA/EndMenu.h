#ifndef MENUEND
#define MENUSEND

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

class EndMenu {
public:

	EndMenu();
	EndMenu(float screenW, float screenH);

	void Render(sf::RenderWindow& window);

	~EndMenu();

private:
	sf::Texture* image;
	sf::Sprite* sprite;
	sf::Font* font;
	sf::Text* text;
	sf::Text* texte;

	float screenW, screenH;
};

#endif