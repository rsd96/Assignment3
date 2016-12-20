#include "Ship.h"
#include "Shot.h"
#include <iostream>

Ship::Ship()
{
}

Ship::Ship(std::string name, unsigned int x, unsigned int y, std::string texturePath)
{
	m_name = name;

	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
	m_sprite.setPosition(x, y);
	m_sprite.scale(0.2f, 0.2f);

	m_health = 100;
	m_healthBar.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, 7));
	m_healthBar.setFillColor(sf::Color::Green);
	m_healthBar.setPosition(this->getSprite().getPosition().x - m_healthBar.getSize().x / 2, this->getSprite().getPosition().y + this->getSprite().getGlobalBounds().height / 2);
	
}


Ship::~Ship()
{
}

void Ship::setSprite(std::string texturePath) {

	m_texture.loadFromFile(texturePath);
	sf::IntRect rect;
	rect.width = m_texture.getSize().x;
	rect.height = m_texture.getSize().y;
	m_sprite.setTextureRect(rect);
	m_sprite.setTexture(m_texture);
}

void Ship::forward(sf::RenderWindow& window)
{

	double rad = (m_sprite.getRotation() - 90) * 3.142 / 180.0;
	double x = cos(rad) * 0.25;
	double y = sin(rad) * 0.25;

	// Checking if the ship is gonna go out of the window
	if (m_sprite.getPosition().x + x + m_sprite.getGlobalBounds().width / 2 < window.getSize().x
		&& m_sprite.getPosition().y + y + m_sprite.getGlobalBounds().height / 2 < window.getSize().y
		&& m_sprite.getPosition().x + x - m_sprite.getGlobalBounds().width / 2 > 0
		&& m_sprite.getPosition().y + y - m_sprite.getGlobalBounds().height / 2 > 0) {

		m_sprite.move(x, y);

		m_healthBar.setPosition(this->getSprite().getPosition().x - m_healthBar.getSize().x / 2, this->getSprite().getPosition().y + this->getSprite().getGlobalBounds().height / 2);

	}
}

void Ship::rotate(float degreeOffset)
{
	m_sprite.rotate(degreeOffset);
}

void Ship::hit(int damage)
{

	m_healthBar.setFillColor(sf::Color::Red);
	m_health -= damage;
	m_healthBar.setScale((float)m_health / 100, 1.0); // reduces health bar relative to health variable 

}

Shot Ship::shoot()
{
	Shot newShot(this);
	return newShot;
}

void Ship::drawShip(sf::RenderWindow & window)
{

	if ((flying) && (flying != diff)) {
		setSprite("textures/Ship_Locust_Fly.png");
		diff = flying;
	} else if ((!flying) && (flying != diff)) {
		setSprite("textures/Ship_Locust_Stalled.png");
		diff = flying;
	}
	window.draw(this->getSprite());
	window.draw(m_healthBar);
}

bool Ship::dropDead(sf::RenderWindow& window) {
	
	setSprite("textures/explosion.png");
	int row = m_texture.getSize().x / 4;
	int col = m_texture.getSize().y / 4;
	static int x = 1;
	static int y = 1;


	m_sprite.setOrigin(30, 0);
	m_sprite.setScale(1.0f, 1.0f);
	m_sprite.scale(1.0f, 1.0f);

	if (m_sprite.getPosition().y < window.getSize().y) {
		m_sprite.setTextureRect(sf::IntRect(row * x - row, col * y - col, row, col));
		y++;
		y == 3 ? y = 1 : x++;
		x == 3 ? x = 1 : x = x;
		return true;
	}
	return false;
	

}
