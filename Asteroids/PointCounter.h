#pragma once

#include <SFML/Graphics.hpp>


class PointCounter {

public:
	PointCounter(sf::Vector2f position);
	sf::Text text;

	void AddPoint(int score, std::string desc);
	void MinusPoint(int score, std::string desc);

	void draw(sf::RenderWindow& window);

	int getPoints();

	void restart(int points, std::string desc);

	void setPoints(int points, std::string desc);

private:

	int points = 0;
	sf::Font font;

};