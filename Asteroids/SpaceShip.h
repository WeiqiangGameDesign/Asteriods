#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>

#include <iostream>

#include "Bullet.h"


#define PI 3.14159265


class SpaceShip {

public: 
	SpaceShip(float radius, sf::Vector2f pos);
	~SpaceShip();

	sf::CircleShape shipCircle;
	sf::Vector2f dir;
	float maxSpeed = .3f;
	float curSpeed = 0.f;
	float offsetSpeed;
	bool invincible = false;
	int life = 3;
	sf::Clock invincibleTimer;

	void thrust() {
		if (curSpeed < maxSpeed) {
			curSpeed += 0.0001f;
		}
		//std::cout << "curSpeed: " << curSpeed << std::endl;
	}

	void speedDown() {
		if (curSpeed > 0.01f) {
			curSpeed -= 0.00002f;
		}
	}

	/*
	
	void fire(sf::Vector2f startPos) {
		Bullet bullet(3.f, startPos);
		bullet.fire();
	}
	*/

	void normalize() {
		float mod = dir.x * dir.x + dir.y * dir.y;
		float mag = std::sqrt(mod);
		dir.x /= mag;
		dir.y /= mag;
	}

	sf::Vector2f faceDir() {
		sf::Vector2f res;
		float myRotation = shipCircle.getRotation() - 90.f;
		res.x += cos(myRotation * PI / 180);
		res.y += sin(myRotation * PI / 180);
		return res;
	}

	void move() {
		//std::cout << "shipcircle" << shipCircle.getRotation() << std::endl;
		float myRotation = shipCircle.getRotation() - 90.f;
		dir.x += cos(myRotation * PI / 180) / 2000.f;
		dir.y += sin(myRotation * PI / 180) / 2000.f;
		if (abs(dir.x) > 1 || abs(dir.y) > 1) {
			normalize();
		}
		/*
		std::cout << "dir.x : " << dir.x << std::endl;
		std::cout << "dir.y : " << dir.y << std::endl;
		*/

		shipCircle.move(dir * curSpeed);
	}

	sf::Vector2f getCenter();


};

