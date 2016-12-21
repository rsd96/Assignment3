#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <list>

class Shot;

class Ship
{
public:
	Ship();
	Ship(std::string name, unsigned int x, unsigned int y, std::string texturePath);
	~Ship();


	bool flying = false;
	bool shooting;


	void setSprite(std::string texturePath);
	sf::Sprite getSprite() const { return m_sprite; };
	void forward(sf::RenderWindow& window);
	void rotate(float degreeOffset);
	void hit(int);
	Shot shoot();
	void drawShip(sf::RenderWindow& window);
	int getHealth() { return m_health; };
	std::string getName() { return m_name; };
	bool dropDead(sf::RenderWindow& window);


private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	int m_health;
	sf::RectangleShape m_healthBar;
	bool canMove = true;
	bool status = false; 
	std::string m_name;

};

