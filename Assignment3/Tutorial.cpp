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

	
	Ship p1("Player 1", window.getSize().x / 3, window.getSize().y / 3, "textures/Ship_Locust_Stalled.png");
	Ship computer("Computer", window.getSize().x / 2, window.getSize().y / 2, "textures/Ship_Locust_Stalled.png");


	float playerPosX = p1.getSprite().getPosition().x;
	float playerPosY = p1.getSprite().getPosition().y;

	bool tutorial = true; 



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

			case Event::MouseMoved:

				break;

			default:
				break;

			}

			// To change texture when ship stops 
			if ((evnt.type == evnt.KeyReleased) && (evnt.key.code == sf::Keyboard::Up)) {
				p1.flying = false;
				jetSound.stop();
			}
		}

		/////////////////////////////////////////////////////
		//	COMPUTER AI 
		/////////////////////////////////////////////////////

		if (explosionSound.getStatus() == Sound::Stopped) {
			srand(time(NULL));

			int r = rand() % 2;


			if (r == 0) {
				if (playerPosX <= window.getSize().x / 2) {
					//if(computer.getSprite().getRotation() >= 0 && computer.getSprite().getRotation() < 180)
					computer.rotate(-0.3f);
				}
				else {
					//if (computer.getSprite().getRotation() >= 180 && computer.getSprite().getRotation() < 360)
					computer.rotate(0.3f);

				}
			}
			else {
				computer.forward(window);
				computer.flying = true;
				if (jetSound.getStatus() != Sound::Playing)
					jetSound.play();
			}

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

		/////////////////////////////////////////////////////
		//	PLAYER 1 CONTROLS 
		/////////////////////////////////////////////////////
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			p1.rotate(-0.3f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			p1.rotate(0.3f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			p1.flying = true;
			p1.forward(window);
			if (jetSound.getStatus() != Sound::Playing)
				jetSound.play();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Slash)) {

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

		/////////////////////////////////////////////////////
		//	MOVING BULLETS AND COLLION DETECTION
		/////////////////////////////////////////////////////

		shots_it = shots.begin();

		while (shots_it != shots.end()) {
			
			// Checking is the bullet has gone out of screen to delete and free memory
			shots_it->move();
			
			if (shots_it->getBullet().getPosition().y > window.getSize().y
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
		computer.drawShip(window);
		// drawing all the shots in the list 
		for (Shot &shot : shots)
		{
			window.draw(shot.getBullet());
		}
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
