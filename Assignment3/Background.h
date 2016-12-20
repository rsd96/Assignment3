#pragma once
#include "SFML\Graphics.hpp"


class Background
{
public:
	Background();
	~Background();

	// Call to draw background of the game 
	void drawBG(sf::RenderWindow& window);

private:
	sf::Texture bgTexture;
	sf::Sprite bg;
	// Array of planet textures to load a differnt map each time game opens  
	sf::Texture planetTexture[3];
	sf::Sprite planet;

};

