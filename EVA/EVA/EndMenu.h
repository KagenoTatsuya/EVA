#ifndef MENUEND
#define MENUSEND

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

class EndMenu {
public:

	EndMenu();

	void Render(sf::RenderWindow& window);

	~EndMenu();

private:
	sf::Texture* image;
	sf::Sprite* sprite;
};

#endif