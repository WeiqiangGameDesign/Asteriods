#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Asteriod {

public:

	enum AsteriodType {
		Nothing,
		Giant,
		Mid,
		Small
	};

	sf::CircleShape asteriod;
	sf::Vector2f dir;
	bool explored = false;
	bool emited = false;
	float speed = .5f;
	AsteriodType asType = Giant;

	Asteriod(float radius, sf::Vector2f startPos);
	~Asteriod();

	void setDir(sf::Vector2f newDir) {
		this->dir = newDir;
		emited = true;
	}

	void setSpeed(float newSpeed) {
		speed = newSpeed;
	}

	void move() {
		//std::cout << "dir.x:" << dir.x << std::endl;
		//std::cout << "dir.y:" << dir.y << std::endl;
		asteriod.move(dir * speed);
	}

	void explore() {
		asteriod.setFillColor(sf::Color::Transparent);
		explored = true;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(asteriod);
	}

};
