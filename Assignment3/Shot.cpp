#include "Shot.h"

Shot::Shot()
{
}

Shot::Shot(Ship *origin)
{
	m_texture.loadFromFile("textures/Bullet_FireRed.png");
	m_bullet.setTexture(m_texture);
	m_bullet.setPosition(origin->getSprite().getPosition().x, origin->getSprite().getPosition().y);
	m_bullet.setRotation(origin->getSprite().getRotation());
	m_bullet.scale(0.3f, 0.3f);
	m_origin = origin;
}

// Copy Constructor, to prevent loss of sprite texture when adding to vector in game
Shot::Shot(const Shot &shot) : m_texture(shot.m_texture), m_bullet(shot.m_bullet), m_origin(shot.m_origin)
{
	m_bullet.setTexture(m_texture);
}


Shot::~Shot()
{
}

void Shot::move()
{
	float rad = (m_bullet.getRotation() - 90) * 3.142 / 180.0;
	m_bullet.move(cos(rad), sin(rad));
}

