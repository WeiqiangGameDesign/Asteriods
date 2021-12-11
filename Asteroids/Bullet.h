#pragma once


#include <SFML/Graphics.hpp>

class Bullet {

public: 

	sf::CircleShape bullet;
	sf::Vector2f bulletDir;
	bool isFired = false;
	bool disappeared = false;
	float firedTime;
	float speed = 0.6f;
	sf::Clock clock;

	Bullet(float radius, sf::Vector2f startPos); 
	~Bullet();

	void setBulletDir(sf::Vector2f newBulletDir) {
		bulletDir = newBulletDir;
	}

	void fire() {
		bullet.move(bulletDir * speed);
	}

	void draw(sf::RenderWindow &window) {
		window.draw(bullet);
	}

};

