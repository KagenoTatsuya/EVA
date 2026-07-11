#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Button {
protected:
	sf::Font font;
	sf::Text text = sf::Text(font, "", 20);
	sf::RectangleShape button;
	bool isHovered = false;

	sf::Texture textureNormal;
	sf::Texture textureHover;

public:
	Button();
	virtual ~Button() = default;
	virtual float GetPosX() = 0;
	virtual float GetRightX() = 0;
	virtual float GetPosY() = 0;
	virtual float GetBottomY() = 0;
	virtual void SetText(std::string textStirng, float posx, float posy, float width, float height, sf::Font& font);
	virtual void Render(sf::RenderWindow& window) = 0;

	void LoadTextures(const std::string& normalPath, const std::string& hoverPath);
	void UpdateHover(sf::RenderWindow& window, sf::View& uiView);
	virtual void OnHover() = 0;
	virtual void OnNormal() = 0;
	bool IsHovered() const { return isHovered; }
};

class Exit : public Button {
	float posx;
	float posy;
	float width;
	float height;
public:
	Exit();
	virtual ~Exit() {};
	inline float GetPosX() override { return posx; };
	inline float GetRightX() override { return posx + width; };
	inline float GetPosY() override { return posy; };
	inline float GetBottomY() override { return posy + height; };
	void Render(sf::RenderWindow& window) override;
	void OnHover() override;
	void OnNormal() override;
};

class Start : public Button {
	float posx;
	float posy;
	float width;
	float height;
public:
	Start();
	virtual ~Start() {};
	inline float GetPosX() override { return posx; };
	inline float GetRightX() override { return posx + width; };
	inline float GetPosY() override { return posy; };
	inline float GetBottomY() override { return posy + height; };
	void Render(sf::RenderWindow& window) override;
	void OnHover() override;
	void OnNormal() override;
};

class Continue : public Button {
	float posx;
	float posy;
	float width;
	float height;
public:
	Continue();
	virtual ~Continue() {};
	inline float GetPosX() override { return posx; };
	inline float GetRightX() override { return posx + width; };
	inline float GetPosY() override { return posy; };
	inline float GetBottomY() override { return posy + height; };
	void Render(sf::RenderWindow& window) override;
	void OnHover() override;
	void OnNormal() override;
};

class Zombie : public Button {
	float posx;
	float posy;
	float width;
	float height;
public:
	Zombie();
	virtual ~Zombie() {};
	inline float GetPosX() override { return posx; };
	inline float GetRightX() override { return posx + width; };
	inline float GetPosY() override { return posy; };
	inline float GetBottomY() override { return posy + height; };
	void Render(sf::RenderWindow& window) override;
	void OnHover() override;
	void OnNormal() override;
};

class Arene : public Button {
	float posx;
	float posy;
	float width;
	float height;
public:
	Arene();
	virtual ~Arene() {};
	inline float GetPosX() override { return posx; };
	inline float GetRightX() override { return posx + width; };
	inline float GetPosY() override { return posy; };
	inline float GetBottomY() override { return posy + height; };
	void Render(sf::RenderWindow& window) override;
	void OnHover() override;
	void OnNormal() override;
};
