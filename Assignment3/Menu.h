#pragma once
#include "Screens.h"
#include "SFML\Audio.hpp"

// Number of menu options 
#define totalMenuItems 3

class Menu : public Screens {
public:
	Menu();
	int Run(sf::RenderWindow &window);

private:

	sf::Music bgMusic;
	int index;
	sf::Font font;
	sf::Texture bg;
	sf::Sprite menuBG;
	std::string menuText[totalMenuItems] = { "TUTORIAL", "PLAY", "EXIT" };
	sf::Text menuOptions[totalMenuItems];

};

