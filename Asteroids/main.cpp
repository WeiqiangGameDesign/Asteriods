#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SpaceShip.h"
#include "CollisionDetectUtil.h"
#include "Asteriod.h"
#include "Menu.h"
#include "PointCounter.h"

#include <iostream>

enum States {
	MENU_STATE,
	PLAY,
	RESULT,
	QUIT
};


int main()
{	

	int gameMode = MENU_STATE;

	bool nextLevel = false;

	bool isThrusting = false;

	float nextSpeed = 0.5f;

    // setup Clock
    sf::Clock clock;

    srand(time(NULL));

	clock.restart();

    bool isFiring = false;
	bool gameRestart = false;
    float windowWidth = 1500.f;
    float windowHeight = 1500.f;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Asteriods");
    //window.setFramerateLimit(60);
    //sf::CircleShape shape(50.f);
	//shape.setPosition(50, 50);
    //shape.setFillColor(sf::Color::White);
    //shape.setFillColor(sf::Color::Green);
	//shape.setOrigin(sf::Vector2f(50, 50));
	SpaceShip ship(50, sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f));
	ship.shipCircle.setOrigin(sf::Vector2f(ship.shipCircle.getRadius(), ship.shipCircle.getRadius()));
	//CollisionDetectUtil::setBoundary(0, windowWidth, 0, windowHeight);

	
	sf::SoundBuffer brickBuffer;
	if (!brickBuffer.loadFromFile("./audios/beep.ogg")) {
		std::cout << "fail to load beep.ogg" << std::endl;
		return -1;
	}
	sf::Sound brickSound;
	brickSound.setBuffer(brickBuffer);

	sf::SoundBuffer powerBuffer;
	if (!powerBuffer.loadFromFile("./audios/Powerup.wav")) {
		std::cout << "fail to load power.wav" << std::endl;
		return -1;
	}

	sf::Sound powerUpSound;
	powerUpSound.setBuffer(powerBuffer);

	sf::SoundBuffer exploredBuffer;
	if (!exploredBuffer.loadFromFile("./audios/explored.wav")) {
		std::cout << "fail to load explored.wav" << std::endl;
		return -1;
	}

	sf::Sound exploredSound;
	exploredSound.setBuffer(exploredBuffer);

	sf::SoundBuffer deathBuffer;
	if (!deathBuffer.loadFromFile("./audios/death.flac")) {
		std::cout << "fail to load death.flac" << std::endl;
		return -1;
	}

	sf::Sound deathSound;
	deathSound.setBuffer(deathBuffer);

	sf::SoundBuffer thrustBuffer;
	if (!thrustBuffer.loadFromFile("./audios/thrust.flac")) {
		std::cout << "fail to load thrust.flac" << std::endl;
		return -1;
	}

	sf::Sound thrustSound;
	thrustSound.setBuffer(thrustBuffer);


	Menu menu(window.getSize().x, window.getSize().y);
	menu.isOpen = true;

	PointCounter lifeCounter(sf::Vector2f(30.f, windowHeight - 36.f));
	lifeCounter.setPoints(3, "life: ");
	PointCounter scoreCounter(sf::Vector2f(windowWidth - 240.f, 36.f));
	scoreCounter.setPoints(0, "score: ");

	// setup result
	sf::Text resText;
	sf::Font resFont;
	if (!resFont.loadFromFile("./fonts/open-sans/OpenSans-Light.ttf")) {
		std::cout << "resfont load error" << std::endl;
	}
	resText.setFont(resFont);
	resText.setPosition(windowWidth / 2 - 360.f, windowHeight / 2);


    sf::Texture shipTexture;
    //shipTexture.loadFromFile("./images/spaceship.png");
    shipTexture.loadFromFile("./images/spaceship.png");
    //shape.setTexture(&shipTexture);
    ship.shipCircle.setTexture(&shipTexture);

    sf::Texture shieldTexture;
    //shipTexture.loadFromFile("./images/spaceship.png");
    shieldTexture.loadFromFile("./images/shield.png");
    //shape.setTexture(&shipTexture);

    sf::Texture thrustTexture;
    thrustTexture.loadFromFile("./images/thrust.png");

	sf::Texture giantTexture;
	giantTexture.loadFromFile("./images/giant.png");

	sf::Texture midTexture;
	midTexture.loadFromFile("./images/mid.png");

	sf::Texture smallTexture;
	smallTexture.loadFromFile("./images/small.png");


	std::vector<Bullet> bulletVec;
	for (int i = 0; i < 5; i++) {
		Bullet newBullet(5.f, ship.getCenter());
		newBullet.bullet.setFillColor(sf::Color::Transparent);
		newBullet.isFired = false;
		newBullet.disappeared = false;
		bulletVec.push_back(newBullet);
	}

	std::vector<Asteriod> asteriods;
	for (int i = 0; i < 3; i++) {
		float asWidth = rand() % 1200 + 100;
		float asHeight = rand() % 1200 + 100;
		while (asWidth < ship.shipCircle.getPosition().x + 200.f &&
			asWidth > ship.shipCircle.getPosition().x &&
			asHeight < ship.shipCircle.getPosition().y + 200.f &&
			asHeight > ship.shipCircle.getPosition().y) {
			asWidth += 200.f;
			asHeight += 200.f;
		}
		Asteriod newAsteroid(80.f, sf::Vector2f(asWidth, asHeight));
		float asX = (float)(rand() % 20 - 10) / (float)100.f;
		float asY = (float)(rand() % 20 - 10) / (float)100.f;
		std::cout << asX << std::endl;
		std::cout << asY << std::endl;

		newAsteroid.setDir(sf::Vector2f(asX, asY));
		newAsteroid.setSpeed(.5f);
		newAsteroid.asteriod.setTexture(&giantTexture);
		asteriods.push_back(newAsteroid);
	}


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (gameMode == MENU_STATE) {
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
					case sf::Keyboard::W:
						menu.MoveUp();
						break;
					case sf::Keyboard::Down:
					case sf::Keyboard::S:
						menu.MoveDown();
					case sf::Keyboard::Return:
						switch (menu.getPressedItem())
						{
						case 0:
							std::cout << "Play" << std::endl;
							menu.isOpen = false;
							gameMode = PLAY;
							break;
						case 1:
							std::cout << "Exit" << std::endl;
							gameMode = QUIT;
							window.close();
							break;
						}
					}
				}
				else if (gameMode == PLAY) {
					if (event.key.code == sf::Keyboard::Escape) {
						gameRestart = true;
						gameMode = MENU_STATE;
						menu.isOpen = true;
					}
				}
			}


		}

		// window scale set up
		sf::View view;
		auto windowSize = window.getSize();
		view.reset(sf::FloatRect(0.f, 0.f, windowSize.x, windowSize.y));
		window.setView(view);
		if (gameMode == MENU_STATE) {
			window.clear();
			menu.draw(window, !menu.isOpen);
		}else if (gameMode == PLAY) {

			for (int i = 0; i < asteriods.size(); i++) {
				if (asteriods[i].explored == false) {
					break;
				}
				if (i == asteriods.size() - 1) {
					if (asteriods[i].explored == true) {
						nextLevel = true;
					}
				}
			}

			if (gameRestart == true) {
				ship.shipCircle.setPosition( sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f));
				ship.shipCircle.setRotation(0);
				ship.dir = sf::Vector2f(0, 0);
				ship.invincibleTimer.restart();
				gameRestart = false;

				asteriods = std::vector<Asteriod>();
				for (int i = 0; i < 3; i++) {
					float asWidth = rand() % 1200 + 100;
					float asHeight = rand() % 1200 + 100;
					while (asWidth < ship.shipCircle.getPosition().x + 200.f &&
						asWidth > ship.shipCircle.getPosition().x &&
						asHeight < ship.shipCircle.getPosition().y + 200.f &&
						asHeight > ship.shipCircle.getPosition().y) {
						asWidth += 200.f;
						asHeight += 200.f;
					}
					Asteriod newAsteroid(80.f, sf::Vector2f(asWidth, asHeight));
					newAsteroid.asteriod.setTexture(&giantTexture);
					float asX = (float)(rand() % 20 - 10) / (float)100.f;
					float asY = (float)(rand() % 20 - 10) / (float)100.f;
					std::cout << asX << std::endl;
					std::cout << asY << std::endl;

					newAsteroid.setDir(sf::Vector2f(asX, asY));
					newAsteroid.setSpeed(.5f);
					asteriods.push_back(newAsteroid);
				}

				//ship.shipCircle.setOrigin(sf::Vector2f(ship.shipCircle.getRadius(), ship.shipCircle.getRadius()));
			}
			else if (nextLevel) {
				powerUpSound.play();
				nextLevel = false;
				ship.shipCircle.setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f));
				ship.shipCircle.setRotation(0);
				ship.dir = sf::Vector2f(0, 0);
				ship.invincibleTimer.restart();
				gameRestart = false;

				asteriods = std::vector<Asteriod>();
				for (int i = 0; i < 4; i++) {
					float asWidth = rand() % 1200 + 100;
					float asHeight = rand() % 1200 + 100;
					while (asWidth < ship.shipCircle.getPosition().x + 200.f &&
						asWidth > ship.shipCircle.getPosition().x &&
						asHeight < ship.shipCircle.getPosition().y + 200.f &&
						asHeight > ship.shipCircle.getPosition().y) {
						asWidth += 200.f;
						asHeight += 200.f;
					}
					Asteriod newAsteroid(80.f, sf::Vector2f(asWidth, asHeight));
					newAsteroid.asteriod.setTexture(&giantTexture);
					float asX = (float)(rand() % 20 - 10) / (float)100.f;
					float asY = (float)(rand() % 20 - 10) / (float)100.f;
					std::cout << asX << std::endl;
					std::cout << asY << std::endl;

					newAsteroid.setDir(sf::Vector2f(asX, asY));
					nextSpeed += 0.3f;
					newAsteroid.setSpeed(nextSpeed);
					asteriods.push_back(newAsteroid);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
				sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//shape.rotate(-0.1f);
				ship.shipCircle.rotate(-0.05f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
				sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				//shape.setOrigin(sf::Vector2f(25, 25));
				//shape.rotate(0.1f);
				ship.shipCircle.rotate(0.05f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
				sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
				ship.thrust();
				if (clock.getElapsedTime().asSeconds() > 1.f) {
					isThrusting = true;
					thrustSound.play();
					ship.shipCircle.setTexture(&thrustTexture);
					clock.restart();
				}
			}


			if (CollisionDetectUtil::topBoundaryCheck(ship.shipCircle)) {
				ship.shipCircle.setPosition(sf::Vector2f(ship.shipCircle.getPosition().x, windowHeight));
			}
			else if (CollisionDetectUtil::leftBoundaryCheck(ship.shipCircle)) {
				ship.shipCircle.setPosition(sf::Vector2f(windowWidth, ship.shipCircle.getPosition().y));
			}
			else if (CollisionDetectUtil::bottomBoundaryCheck(ship.shipCircle)) {
				ship.shipCircle.setPosition(sf::Vector2f(ship.shipCircle.getPosition().x, 0));
			}
			else if (CollisionDetectUtil::rightBoundaryCheck(ship.shipCircle)) {
				ship.shipCircle.setPosition(sf::Vector2f(0, ship.shipCircle.getPosition().y));
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				float delta_s = clock.getElapsedTime().asSeconds();
				if (delta_s > 0.02f) {
					isFiring = true;
				}
				clock.restart();
			}

			//std::cout << "bulletVec" << bulletVec.size() << std::endl;

			if (isFiring == true) {
				//	Bullet newBullet(5.f, ship.getCenter());
				//	bulletVec.push_back(newBullet);
				for (int i = 0; i < bulletVec.size(); i++) {
					//std::cout << "isFired" << bulletVec[i].isFired << std::endl;
					//std::cout << "disappeared" << bulletVec[i].disappeared << std::endl;
					if (bulletVec[i].isFired == false && bulletVec[i].disappeared == false) {
						std::cout << "num of bullet" << i << std::endl;
						brickSound.play();
						bulletVec[i].bullet.setPosition(ship.getCenter());
						bulletVec[i].bullet.setFillColor(sf::Color::Red);
						bulletVec[i].setBulletDir(ship.faceDir());
						bulletVec[i].isFired = true;
						bulletVec[i].clock.restart();
						std::cout << "firing!" << std::endl;
						break;
					}
				}
				isFiring = false;
			}
			// reuse bullets
			for (int i = 0; i < bulletVec.size(); i++) {
				float delta_s = bulletVec[i].clock.getElapsedTime().asSeconds();
				if (delta_s > 1.f) {
					if (bulletVec[i].isFired == true) {
						bulletVec[i].disappeared = true;
						bulletVec[i].bullet.setFillColor(sf::Color::Transparent);
					}
				}
			}

			for (int i = 0; i < bulletVec.size(); i++) {
				if ((bulletVec[i].isFired == true && bulletVec[i].disappeared == true)) {
					bulletVec[i].isFired = false;
					bulletVec[i].disappeared = false;
				}
			}

			ship.move();
			ship.speedDown();

			// boundary
			for (int i = 0; i < asteriods.size(); i++) {
				asteriods[i].move();
				if (CollisionDetectUtil::topBoundaryCheck(asteriods[i].asteriod)) {
					asteriods[i].asteriod.setPosition(sf::Vector2f(asteriods[i].asteriod.getPosition().x, windowHeight));
				}
				else if (CollisionDetectUtil::leftBoundaryCheck(asteriods[i].asteriod)) {
					asteriods[i].asteriod.setPosition(sf::Vector2f(windowWidth, asteriods[i].asteriod.getPosition().y));
				}
				else if (CollisionDetectUtil::bottomBoundaryCheck(asteriods[i].asteriod)) {
					asteriods[i].asteriod.setPosition(sf::Vector2f(asteriods[i].asteriod.getPosition().x, 0 - asteriods[i].asteriod.getRadius() * 2));
				}
				else if (CollisionDetectUtil::rightBoundaryCheck(asteriods[i].asteriod)) {
					asteriods[i].asteriod.setPosition(sf::Vector2f(0 - asteriods[i].asteriod.getRadius() * 2, asteriods[i].asteriod.getPosition().y));
				}
			}


			// bullet boundary
			for (int i = 0; i < bulletVec.size(); i++) {
				bulletVec[i].fire();
				if (!bulletVec[i].disappeared && CollisionDetectUtil::topBoundaryCheck(bulletVec[i].bullet)) {
					bulletVec[i].bullet.setPosition(sf::Vector2f(bulletVec[i].bullet.getPosition().x, windowHeight));
				}
				else if (!bulletVec[i].disappeared && CollisionDetectUtil::leftBoundaryCheck(bulletVec[i].bullet)) {
					bulletVec[i].bullet.setPosition(sf::Vector2f(windowWidth, bulletVec[i].bullet.getPosition().y));
				}
				else if (!bulletVec[i].disappeared && CollisionDetectUtil::bottomBoundaryCheck(bulletVec[i].bullet)) {
					bulletVec[i].bullet.setPosition(sf::Vector2f(bulletVec[i].bullet.getPosition().x, 0 - bulletVec[i].bullet.getRadius() * 2));
				}
				else if (!bulletVec[i].disappeared && CollisionDetectUtil::rightBoundaryCheck(bulletVec[i].bullet)) {
					bulletVec[i].bullet.setPosition(sf::Vector2f(0 - bulletVec[i].bullet.getRadius() * 2, bulletVec[i].bullet.getPosition().y));
				}
			}

			// bullet and asteriods collision
			for (int i = 0; i < bulletVec.size(); i++) {
				for (int j = 0; j < asteriods.size(); j++) {
					if ((bulletVec[i].isFired && !bulletVec[i].disappeared)
						&& asteriods[j].explored == false
						&& CollisionDetectUtil::isCircleCircleIntersected(bulletVec[i].bullet, asteriods[j].asteriod)) {
						bulletVec[i].bullet.setFillColor(sf::Color::Transparent);
						exploredSound.play();
						bulletVec[i].disappeared = true;
						asteriods[j].asteriod.setFillColor(sf::Color::Transparent);
						asteriods[j].explored = true;
					}
				}
			}


			// break giant asteroids
			for (int i = 0; i < asteriods.size(); i++) {
				if (asteriods[i].explored && asteriods[i].asType == asteriods[i].Giant) {
					scoreCounter.AddPoint(200, "score: ");
					Asteriod newAsteroid1(40.f, sf::Vector2f(
						asteriods[i].asteriod.getPosition().x - 40.f,
						asteriods[i].asteriod.getPosition().y));
					newAsteroid1.asType = newAsteroid1.Mid;
					newAsteroid1.asteriod.setTexture(&midTexture);
					Asteriod newAsteroid2(40.f, sf::Vector2f(
						asteriods[i].asteriod.getPosition().x + 40.f,
						asteriods[i].asteriod.getPosition().y));
					newAsteroid2.asType = newAsteroid2.Mid;
					newAsteroid2.asteriod.setTexture(&midTexture);
					float asX = (float)(rand() % 20 - 10) / (float)100.f;
					float asY = (float)(rand() % 20 - 10) / (float)100.f;
					/*
					std::cout << asX << std::endl;
					std::cout << asY << std::endl;
					*/
					newAsteroid1.setDir(sf::Vector2f(asX, asY));
					asX = (float)(rand() % 20 - 10) / (float)100.f;
					asY = (float)(rand() % 20 - 10) / (float)100.f;
					newAsteroid2.setDir(sf::Vector2f(asX, asY));

					asteriods.push_back(newAsteroid1);
					asteriods.push_back(newAsteroid2);
					asteriods[i].asType = asteriods[i].Nothing;
				}
				else if (asteriods[i].explored && asteriods[i].asType == asteriods[i].Mid) {
					scoreCounter.AddPoint(150, "score: ");
					Asteriod newAsteroid1(20.f, sf::Vector2f(
						asteriods[i].asteriod.getPosition().x - 40.f,
						asteriods[i].asteriod.getPosition().y));
					newAsteroid1.asType = newAsteroid1.Small;
					newAsteroid1.asteriod.setTexture(&smallTexture);
					Asteriod newAsteroid2(20.f, sf::Vector2f(
						asteriods[i].asteriod.getPosition().x + 40.f,
						asteriods[i].asteriod.getPosition().y));
					newAsteroid2.asType = newAsteroid2.Small;
					newAsteroid2.asteriod.setTexture(&smallTexture);
					float asX = (float)(rand() % 20 - 10) / (float)100.f;
					float asY = (float)(rand() % 20 - 10) / (float)100.f;
					/*
					std::cout << asX << std::endl;
					std::cout << asY << std::endl;
					*/
					newAsteroid1.setDir(sf::Vector2f(asX, asY));
					asX = (float)(rand() % 20 - 10) / (float)100.f;
					asY = (float)(rand() % 20 - 10) / (float)100.f;
					newAsteroid2.setDir(sf::Vector2f(asX, asY));

					asteriods.push_back(newAsteroid1);
					asteriods.push_back(newAsteroid2);
					asteriods[i].asType = asteriods[i].Nothing;
				}
				else if (asteriods[i].explored && asteriods[i].asType == asteriods[i].Small) {
					scoreCounter.AddPoint(100, "score: ");
					asteriods[i].asType = asteriods[i].Nothing;
				}
			}

			// collision of asteriods
			for (int i = 0; i < asteriods.size() - 1; i++) {
				for (int j = i + 1; j < asteriods.size(); j++) {
					if (!asteriods[i].explored &&
						!asteriods[j].explored &&
						CollisionDetectUtil::isCircleCircleIntersected(asteriods[i].asteriod, asteriods[j].asteriod)) {
						asteriods[i].dir = -asteriods[i].dir;
						asteriods[j].dir = -asteriods[j].dir;
						break;
					}
				}
			}

			if (clock.getElapsedTime().asSeconds() > 0.5f) {
				isThrusting = false;
			}

			if (ship.invincibleTimer.getElapsedTime().asSeconds() > 3.f) {
				ship.invincible = false;
			}

			if (ship.invincible == true) {
				ship.shipCircle.setTexture(&shieldTexture);
			}
			else if (isThrusting == false) {
				ship.shipCircle.setTexture(&shipTexture);
			}
			else {
				ship.shipCircle.setTexture(&thrustTexture);
			}

			for (int i = 0; i < asteriods.size(); i++) {
				if (!ship.invincible && !asteriods[i].explored && CollisionDetectUtil::isCircleCircleIntersected(ship.shipCircle, asteriods[i].asteriod)) {
					if (ship.life > 0) {
						deathSound.play();
						ship.life--;
						lifeCounter.MinusPoint(1, "life: ");
						std::cout << "ship.life: " << ship.life << std::endl;
						ship.invincible = true;
						ship.shipCircle.setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f));
						ship.dir = sf::Vector2f(0, 0);
						ship.invincibleTimer.restart();
						if (ship.life <= 0) {
							gameMode = RESULT;
							break;
						}
					}
				}
			}

			window.clear();

			///window.draw(shape);
			window.draw(ship.shipCircle);
			for (int i = 0; i < bulletVec.size(); i++) {
				bulletVec[i].draw(window);
			}

			for (int i = 0; i < asteriods.size(); i++) {
				asteriods[i].draw(window);
			}

			lifeCounter.draw(window);
			scoreCounter.draw(window);

		}
		else if (gameMode == RESULT) {
			window.clear();
			if (lifeCounter.getPoints() == 0) {
				resText.setString("Your Final Score:" + std::to_string(scoreCounter.getPoints()) + "(Press Enter Start Again)");
				resText.setCharacterSize(50.f);
				window.draw(resText);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					gameMode = MENU_STATE;
					menu.isOpen = true;
					lifeCounter.restart(3, "life: ");
					scoreCounter.restart(0, "score: ");
					ship.life = 3;
					gameRestart = true;
					//powerUpSound.play();
				}

			}

		}

		window.display();
	}

	return 0;
}