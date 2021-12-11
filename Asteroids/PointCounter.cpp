#include "PointCounter.h"
#include <iostream>

PointCounter::PointCounter(sf::Vector2f position)
{
	if (!font.loadFromFile("./fonts/open-sans/OpenSans-Light.ttf")) {
		// hanle error
		std::cout << "font load error!" << std::endl;
	}

	text.setFont(font);
	std::string content = std::to_string(points);
	text.setString(content);
	text.setFillColor(sf::Color::White);
	text.setPosition(position);
	text.setCharacterSize(30.f);
	points = 0;
}

void PointCounter::AddPoint(int score, std::string desc)
{
	this->points += score;
	std::string content = desc + std::to_string(points);
	text.setString(content);
}

void PointCounter::MinusPoint(int score, std::string desc) {
	this->points -= score;
	std::string content = desc + std::to_string(points);
	text.setString(content);
}

void PointCounter::draw(sf::RenderWindow& window)
{
	window.draw(text);
}

int PointCounter::getPoints()
{
	return this->points;
}

void PointCounter::restart(int points, std::string desc)
{
	this->points = points;
	std::string content = desc + std::to_string(points);
	text.setString(content);
}

void PointCounter::setPoints(int points, std::string desc)
{
	this->points = points;
	std::string content = desc + std::to_string(points);
	text.setString(content);
}