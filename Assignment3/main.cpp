#include <SFML\Graphics.hpp>
#include <iostream> 
#include <math.h>
#include "Screens.h"
#include "Menu.h"
#include "PlayGame.h"
#include "Tutorial.h"
#include "EndGame.h"
#define PI  3.14

using namespace sf;


int main() {
	Tutorial tutorial;
	Menu menu;
	PlayGame game;
	EndGame endGame;
	Screens* screens[] = { &menu, &tutorial, &game, &endGame };
	
	sf::RenderWindow window(sf::VideoMode(1024, 600), "Space Shooter", sf::Style::Default);
	int currScreen = 0; // Sets menu screen as the starting screen 
	while (currScreen >= 0) {
		currScreen = screens[currScreen]->Run(window);
	}
	return 0;
}