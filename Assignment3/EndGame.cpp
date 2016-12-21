#include "EndGame.h"
#include <iostream>

// defining static variable
Ship EndGame::m_winner;

EndGame::EndGame() {
	m_bg.loadFromFile("textures/endScreenBG.jpg");
	m_bg.setSmooth(true);
	m_endBG.setTexture(m_bg);
	m_font.loadFromFile("space age.ttf");

	m_endText.setFont(m_font);
	m_endText.setFillColor(sf::Color::Green);
	m_endText.setStyle(sf::Text::Style::Bold);
	m_endText.setOutlineThickness(1);
	m_endText.setOutlineColor(sf::Color::Black);
	m_endText.setStyle(sf::Text::Style::Bold);

	m_restartText.setString("restart");
	m_restartText.setFont(m_font);
	m_restartText.setFillColor(sf::Color::White);
	m_restartText.setOutlineThickness(1);
	m_restartText.setOutlineColor(sf::Color::Black);

	m_mainMenuText.setString("main menu");
	m_mainMenuText.setFont(m_font);
	m_mainMenuText.setFillColor(sf::Color::White);
	m_mainMenuText.setOutlineThickness(1);
	m_mainMenuText.setOutlineColor(sf::Color::Black);

}


EndGame::~EndGame()
{
}

int EndGame::Run(sf::RenderWindow& window)
{
	// Resizing background picture relative to the window size 
	m_endBG.scale(window.getSize().x / m_endBG.getGlobalBounds().width, window.getSize().y / m_endBG.getGlobalBounds().height);

	m_endText.setString(m_winner.getName() + " wins !");
	m_endText.setPosition(window.getSize().x / 2 - m_endText.getGlobalBounds().width / 2, window.getSize().y * 0.25);

	m_restartText.setPosition(window.getSize().x * 0.25 - m_restartText.getGlobalBounds().width / 2, window.getSize().y / 2);
	m_mainMenuText.setPosition(window.getSize().x * 0.75 - m_mainMenuText.getGlobalBounds().width / 2, window.getSize().y / 2);

	int index = 1; // to change text color as user chooses option

	while (window.isOpen()) {
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {

			case sf::Event::Closed:
				return -1;
				break;

			case sf::Event::Resized:
				std::cout << evnt.size.width << " " << evnt.size.height << std::endl;
				break;

			case evnt.KeyPressed:
				if (evnt.key.code == sf::Keyboard::Right) {
					(index < 2) ? index++ : index = index;
				}
				else if (evnt.key.code == sf::Keyboard::Left) {
					(index > 1) ? index-- : index = index;
				}
				else if (evnt.key.code == sf::Keyboard::Return) {
					std::cout << index << std::endl;
					if (index == 1)
						return SCREENS::game;
					else
						return SCREENS::menu;
				}

				break;

			default:
				break;
			}
		}


		if (index == 1) {
			m_restartText.setFillColor(sf::Color::Red);
			m_mainMenuText.setFillColor(sf::Color::White);
		}
		else {
			m_mainMenuText.setFillColor(sf::Color::Red);
			m_restartText.setFillColor(sf::Color::White);
		}

		window.clear();
		window.draw(m_endBG);
		window.draw(m_endText);
		window.draw(m_restartText);
		window.draw(m_mainMenuText);
		window.display();
	}
	return 0;
}
