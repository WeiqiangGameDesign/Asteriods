#include "Asteriod.h"



Asteriod::Asteriod(float radius, sf::Vector2f startPos)
{
	asteriod.setRadius(radius);
	asteriod.setPosition(startPos);
}

Asteriod::~Asteriod()
{
}