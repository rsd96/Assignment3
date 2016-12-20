#include "Background.h"
#include <time.h>
#include <cstdlib>


Background::Background() {

	bgTexture.loadFromFile("textures/SpaceBG.png");
	bgTexture.setSmooth(true);
	bg.setTexture(bgTexture);
	
	// Random value to choose a planet each time the game is loaded 
	srand(time(NULL));
	int r = rand() % 2;

	planetTexture[0].loadFromFile("textures/Planet_IceGiant.png");
	planetTexture[1].loadFromFile("textures/Planet_Sun.png");

	planet.setTexture(planetTexture[r]);
	planet.setScale(1.0f, 1.0f);
	planet.scale(0.5f, 0.5f);
	planet.setOrigin(planetTexture[r].getSize().x / 2, planetTexture[r].getSize().y / 2);
}


Background::~Background()
{
}

void Background::drawBG(sf::RenderWindow& window)
{
	planet.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	// Resizing background picture relative to the window size so that background sprite always is full screen
	bg.scale(window.getSize().x / bg.getGlobalBounds().width, window.getSize().y / bg.getGlobalBounds().height);
	window.draw(bg);
	window.draw(planet);
}
