#pragma once

#include <SFML\Graphics.hpp>

// Abstract class representing any screen thats to be shown 

class Screens {
public:
	
	// Values assigned to each screen, return these values to go to the respective screen
	enum SCREENS {
		menu, tutorial, game, endGame, exit = -1
	};
	 
	// Virtual to be defined in every screen, contains the game loop of each screen 
	virtual int Run(sf::RenderWindow &window) = 0;
};