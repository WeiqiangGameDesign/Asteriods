#include "SpaceShip.h"


SpaceShip::SpaceShip(float radius, sf::Vector2f pos)
{
	shipCircle.setRadius(radius);
	shipCircle.setPosition(pos);
}

SpaceShip::~SpaceShip()
{
}

sf::Vector2f SpaceShip::getCenter()
{
	//return shipCircle.getPosition() + sf::Vector2f(shipCircle.getRadius(), shipCircle.getRadius());
	return shipCircle.getPosition();
}
