#pragma once

#include <SFML\Graphics.hpp>
#include "Ship.h"

class Shot
{

public:
	Shot();
	Shot(Ship *origin);
	Shot(const Shot &shot);
	~Shot();

	sf::Sprite getBullet() const { return m_bullet; }
	Ship * getOrigin() const { return m_origin; }
	void move();

private:
	sf::Sprite m_bullet;
	sf::Texture m_texture;
	Ship *m_origin;
};

