#include "Menu.h"
#include <iostream>



Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("./fonts/open-sans/OpenSans-Light.ttf")) {
		//  handle error
		std::cout << "font load error!" << std::endl;
	}

	menu[0].setFont(font);
	menu[0].setString("1. Play(When selected Press Enter Button)");
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setPosition(sf::Vector2f(width / 2 - 50.f, (height / (MAX_NUM_OF_ITEMS + 1)) * 2 - 50.f));


	menu[1].setFont(font);
	menu[1].setString("2. Exit");
	menu[1].setFillColor(sf::Color::White);
	menu[1].setPosition(sf::Vector2f(width / 2 - 50.f, (height / (MAX_NUM_OF_ITEMS + 1)) * 2));


	/*
	menu[2].setFont(font);
	menu[2].setString("3. Exit");
	menu[2].setFillColor(sf::Color::White);
	menu[2].setPosition(sf::Vector2f(width / 2 - 150.f, (height / (MAX_NUM_OF_ITEMS + 1)) * 2 + 50.f));
	*/

	selectedItemIndex = 0;
}

Menu::~Menu()
{

}

void Menu::draw(sf::RenderWindow& window, bool close)
{
	if (!close) {
		for (int i = 0; i < MAX_NUM_OF_ITEMS; i++) {
			window.draw(menu[i]);
		}
	}
}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUM_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex >= 1) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex < MAX_NUM_OF_ITEMS - 1) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

int Menu::getPressedItem()
{
	return selectedItemIndex;
}