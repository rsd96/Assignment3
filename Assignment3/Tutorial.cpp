#include "tutorial.h"
#include <iostream> 
#include <time.h>

using namespace sf;


Tutorial::Tutorial() {
	

	if (!shotBuffer.loadFromFile("sounds/laser_shot.wav"))
		std::cout << "Error loading sound ! " << std::endl;


	if (!jetMoveBuffer.loadFromFile("sounds/aircraft.wav"))
		std::cout << "Error loading jet sound ! " << std::endl;

	if (!explosionBuffer.loadFromFile("sounds/explosion.wav"))
		std::cout << "Error loading explosion sound ! " << std::endl;


	shotSound.setBuffer(shotBuffer);
	jetSound.setBuffer(jetMoveBuffer);
	explosionSound.setBuffer(explosionBuffer);
}



Tutorial::~Tutorial()
{
}

int Tutorial::Run(sf::RenderWindow & window) {

	
	Ship p1("Player 1", window.getSize().x / 2, window.getSize().y - 100, "textures/Ship_Locust_Stalled.png");
	Ship computer("Computer", window.getSize().x / 2, window.getSize().y / 2, "textures/Ship_Locust_Stalled.png");
	computer.rotate(180.0f); // To make the enemy ship point to the player

	float playerPosX = p1.getSprite().getPosition().x;
	float playerPosY = p1.getSprite().getPosition().y;

	// True as long as tutorial prompts are being displayed 
	bool tutorial = true; 

	// All tutorial messages to be displayed 
	std::string tutorialMessages[] = { "Press D to rotate right", "Press A to rotate left",
										"Press W to move forward", "Press space to shoot", 
										"All set, now destroy the enemy ship !" };

	// Values to check which tutorial prompt to display
	enum TUTS {
		rotateR, rotateL, forward, shoot, game
	};

	// Current tutorial prompt being displayed 
	TUTS currTut = rotateR;

	sf::Font font;
	if (!font.loadFromFile("space age.ttf"))
		std::cout << "Failed to load font !" << std::endl;
	
	sf::Text tutorialPrompt; 
	tutorialPrompt.setFont(font);
	tutorialPrompt.setFillColor(sf::Color::White);
	tutorialPrompt.setString(tutorialMessages[0]);
	tutorialPrompt.setPosition(window.getSize().x / 2 - tutorialPrompt.getGlobalBounds().width / 2, 50);

	int randomizer = 1; 

	sf::Keyboard::Key key = sf::Keyboard::D;

	while (window.isOpen()) {

		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {

			case sf::Event::Closed:
				return Screens::exit;
				break;

			case sf::Event::Resized:
				std::cout << evnt.size.width << " " << evnt.size.height << std::endl;
				break;

			default:
				break;

			}

			// Changing the tutorial prompt when a specific key is pressed 
			if (tutorial) {
				 
				if ((evnt.type == evnt.KeyReleased) && (evnt.key.code == key)) {
					switch (key) {
					
					case Keyboard::D:
						key = Keyboard::A;
						currTut = rotateL;
						break;
					
					case Keyboard::A:
						key = Keyboard::W;
						currTut = forward;
						break;

					case Keyboard::W:
						key = Keyboard::Space;
						currTut = shoot;
						break;

					case Keyboard::Space:
						currTut = game;
						tutorial = false; 
						break;
	
					default:
						break;
					}
					tutorialPrompt.setString(tutorialMessages[currTut]);
					tutorialPrompt.setPosition(window.getSize().x / 2 - tutorialPrompt.getGlobalBounds().width / 2, 50);
				}
			}


			// To change texture when ship stops 
			if ((evnt.type == evnt.KeyReleased) && (evnt.key.code == sf::Keyboard::W)) {
				p1.flying = false;
				jetSound.stop();
			}
		}

		/////////////////////////////////////////////////////
		//	COMPUTER AI (KIND OF)
		/////////////////////////////////////////////////////
		if (!tutorial) {
			if (explosionSound.getStatus() == Sound::Stopped) {
				
				randomizer == 1 ? randomizer = 2 : randomizer = 1; 

				// Random value to decide what action to do 
				srand(time(NULL) + randomizer );
				int r = rand() % 2;


				if (r == 0) {
					// If the player is on the left of computer then turn left, else turn right 
					if (playerPosX <= computer.getSprite().getPosition().x)
						computer.rotate(-0.3f);
					else
						computer.rotate(0.3f);
				
				} else {
					
					computer.forward(window);
					computer.flying = true;
					if (jetSound.getStatus() != Sound::Playing)
						jetSound.play();
				}

				// To prevent shooting a bullet at every game loop
				// made to shoot only when the random value generated before is changed to a new random value
				static int shootingRandom = 0;
				int i = shootingRandom;

				srand(time(NULL));
				shootingRandom = rand() % 100;
				if (shootingRandom != i) {

					if (computer.shooting == false) {
						Shot shot = computer.shoot();
						shots.push_back(shot);
						computer.shooting = true;
						shotSound.play();
					}

				}
				else {
					computer.shooting = false;
				}
			}
		}

		/////////////////////////////////////////////////////
		//	PLAYER 1 CONTROLS 
		/////////////////////////////////////////////////////
		if (currTut == rotateL || currTut == game) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				p1.rotate(-0.3f);
			}
		}

		if (currTut == rotateR || currTut == game) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				p1.rotate(0.3f);
			}
		}

		if (currTut == forward || currTut == game) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				p1.flying = true;
				p1.forward(window);
				if (jetSound.getStatus() != Sound::Playing)
					jetSound.play();
			}
		}

		if (currTut == shoot || currTut == game) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (p1.shooting == false) {
					Shot shot = p1.shoot();
					shots.push_back(shot);
					p1.shooting = true;
					shotSound.play();
				}
			}
			else {
				p1.shooting = false;
			}
		}

		/////////////////////////////////////////////////////
		//	MOVING BULLETS AND COLLION DETECTION
		/////////////////////////////////////////////////////

		shots_it = shots.begin();

		while (shots_it != shots.end()) {
			
			// Checking is the bullet has gone out of screen to delete and free memory
			shots_it->move();
			
			if (   shots_it->getBullet().getPosition().y > window.getSize().y
				|| shots_it->getBullet().getPosition().x > window.getSize().x
				|| shots_it->getBullet().getPosition().x < 0
				|| shots_it->getBullet().getPosition().y < 0) {
				
				// erasing shot objects to free up memory
				shots_it = shots.erase(shots_it);

			} else if (shots_it->getBullet().getGlobalBounds().intersects(shots_it->getOrigin()->getSprite().getGlobalBounds())) {
				// Dont do anything if the bullet is within shit its shot from 
			} else if (shots_it->getBullet().getGlobalBounds().intersects(p1.getSprite().getGlobalBounds())) {

				// p1 hit
				p1.hit(10);
				std::cout << "P1 hit" << std::endl;
				std::cout << "p1 health : " << p1.getHealth() << std::endl;
				// erasing shot objects to free up memory
				shots_it = shots.erase(shots_it);

			} else if (shots_it->getBullet().getGlobalBounds().intersects(computer.getSprite().getGlobalBounds())) {
				
				// computer hit
				computer.hit(10);
				std::cout << "Computer hit" << std::endl;
				std::cout << "Computer health : " << computer.getHealth() << std::endl;
				// erasing shot objects to free up memory
				shots_it = shots.erase(shots_it);

			} else {
				shots_it++;
			}
		}

		/////////////////////////////////////////////////////
		//	GO BACK TO MAIN MENU
		/////////////////////////////////////////////////////

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			return SCREENS::menu;
		}


		window.clear();
		background.drawBG(window);
		p1.drawShip(window);
		// bring in the enemy computer ship when tutorial is over 
		if(!tutorial)
			computer.drawShip(window);
		// drawing all the shots in the list 
		for (Shot &shot : shots)
		{
			window.draw(shot.getBullet());
		}
		
		window.draw(tutorialPrompt);
		window.display();

		/////////////////////////////////////////////////////
		//	Checking for the death of a player
		/////////////////////////////////////////////////////

		if (p1.getHealth() <= 0) {
			
			if (explosionSound.getStatus() != Sound::Playing)
				explosionSound.play();
			
			endGame.setWinner(computer);
			p1.dropDead(window);
			
			while (explosionSound.getStatus() == Sound::Stopped) {
				jetSound.stop();
				return SCREENS::endGame;
			}

		} else if (computer.getHealth() <= 0) {
			
			if (explosionSound.getStatus() != Sound::Playing)
				explosionSound.play();
			
			endGame.setWinner(p1);
			computer.dropDead(window);
			
			while (explosionSound.getStatus() != Sound::Playing) {
				jetSound.stop();
				return SCREENS::endGame;
			}
		}
	}
	return 0;
}
