#include "Menu.h"
#include <iostream>
#include <string>

Menu::Menu() {

	bgMusic.openFromFile("sounds/Into_Battle.wav");
	bgMusic.setLoop(true);


	bg.loadFromFile("textures/MenuBG.png");
	bg.setSmooth(true);

	font.loadFromFile("space age.ttf");
	menuBG = sf::Sprite(bg);
	index = 0;
}


int Menu::Run(sf::RenderWindow & window) {
	bgMusic.play();

	// Setting up all menu options
	for (int i = 0; i < totalMenuItems; i++) {
		menuOptions[i].setString(menuText[i]);
		menuOptions[i].setFont(font);
		menuOptions[i].setPosition(static_cast<float>(window.getSize().x / 2 - menuOptions[i].getGlobalBounds().width / 2), static_cast<float>(window.getSize().y / 4 * (i + 1)));
		menuOptions[i].setOutlineColor(sf::Color::Black);
		menuOptions[i].setOutlineThickness(1);
	}

	menuOptions[index].setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		
		sf::Event evnt;
		while (window.pollEvent(evnt)) {

			if ((evnt.type == evnt.KeyPressed) && (evnt.key.code == sf::Keyboard::Down)) {
				menuOptions[index].setFillColor(sf::Color::White);
				menuOptions[index].setStyle(sf::Text::Style::Regular);
				if (index < totalMenuItems - 1)
					index++;

				menuOptions[index].setFillColor(sf::Color::Green);
				menuOptions[index].setStyle(sf::Text::Style::Bold);
			}

			if ((evnt.type == evnt.KeyPressed) && (evnt.key.code == sf::Keyboard::Up)) {
				menuOptions[index].setFillColor(sf::Color::White);
				menuOptions[index].setStyle(sf::Text::Style::Regular);
				if (index > 0)
					index--;

				menuOptions[index].setFillColor(sf::Color::Green);
				menuOptions[index].setStyle(sf::Text::Style::Bold);
			}

			if ((evnt.type == evnt.KeyPressed) && (evnt.key.code == sf::Keyboard::Return)) {
				bgMusic.stop();
				switch (index) {
				case 0:
					return SCREENS::tutorial;
					break;

				case 1:
					return SCREENS::game;
					break;

				case 2:
					return SCREENS::exit;
					break;

				default:

					break;
				}
			}
		}

		window.clear(sf::Color::Black);
		menuBG.scale(window.getSize().x / menuBG.getGlobalBounds().width, window.getSize().y / menuBG.getGlobalBounds().height);
		window.draw(menuBG);
		// Draw all menu options on screen 
		for (int i = 0; i < totalMenuItems; i++)
			window.draw(menuOptions[i]);
		
		window.display();
	}
	return 0;
}


