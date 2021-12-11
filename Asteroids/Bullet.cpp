#include "Bullet.h"


Bullet::Bullet(float radius, sf::Vector2f startPos)
{
	sf::CircleShape tmp(radius);
	bullet = tmp;
	bullet.setRadius(radius);
	bullet.setPosition(startPos);
	bullet.setFillColor(sf::Color::Red);
}

Bullet::~Bullet()
{
}
