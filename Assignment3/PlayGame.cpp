#include "PlayGame.h"
#include "Ship.h"
#include <iostream> 
#include <math.h>

#define PI  3.14
using namespace sf;


PlayGame::PlayGame() {
	
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

int PlayGame::Run(sf::RenderWindow & window) {
	

	Ship p1("Player 1", window.getSize().x / 3, window.getSize().y / 3, "textures/Ship_Locust_Stalled.png");
	Ship p2("Player 2", window.getSize().x / 2, window.getSize().y / 2, "textures/Ship_Locust_Stalled.png");

	/////////////////////////////////////////////////////
	//	Game Loop 
	/////////////////////////////////////////////////////

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

			// To change texture when ship stops 
			if ((evnt.type == evnt.KeyReleased) && (evnt.key.code == sf::Keyboard::Up)) {
				p1.flying = false;
				jetSound.stop();
			}

			if ((evnt.type == evnt.KeyReleased) && (evnt.key.code == sf::Keyboard::W)) {
				p2.flying = false;
				jetSound.stop();
			}
		}

		if (explosionSound.getStatus() == Sound::Stopped) {

			/////////////////////////////////////////////////////
			//	PLAYER 1 CONTROLS 
			/////////////////////////////////////////////////////
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				p1.rotate(-0.3f);


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				p1.rotate(0.3f);


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
			//	PLAYER 2 CONTROLS 
			/////////////////////////////////////////////////////

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				p2.rotate(-0.3f);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				p2.rotate(0.3f);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

				p2.flying = true;
				p2.forward(window);
				if (jetSound.getStatus() != Sound::Playing)
					jetSound.play();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

				if (p2.shooting == false) {

					Shot shot = p2.shoot();
					shots.push_back(shot);
					p2.shooting = true;
					shotSound.play();

				}

			}
			else {
				p2.shooting = false;
			}

		}
		
			/////////////////////////////////////////////////////
			//	MOVING BULLETS AND COLLION DETECTION
			/////////////////////////////////////////////////////

			shots_it = shots.begin();

			while (shots_it != shots.end()) {

				shots_it->move();
				// Checking if bullet has gone out of screen 
				if (shots_it->getBullet().getPosition().y > window.getSize().y
					|| shots_it->getBullet().getPosition().x > window.getSize().x
					|| shots_it->getBullet().getPosition().x < 0
					|| shots_it->getBullet().getPosition().y < 0) {

					// Delete from vector and free up memory if out of screen 
					shots_it = shots.erase(shots_it);
					std::cout << "shots erased" << std::endl;

				} else if (shots_it->getBullet().getGlobalBounds().intersects(shots_it->getOrigin()->getSprite().getGlobalBounds())) {
					// Dont do anything if the bullet is within shit its shot from 
				} else if (shots_it->getBullet().getGlobalBounds().intersects(p1.getSprite().getGlobalBounds())) {

					p1.hit(10);
					std::cout << "P1 hit" << std::endl;
					std::cout << "p1 health : " << p1.getHealth() << std::endl;
					shots_it = shots.erase(shots_it);

				} else if (shots_it->getBullet().getGlobalBounds().intersects(p2.getSprite().getGlobalBounds())) {

					p2.hit(10);
					std::cout << "P2 hit" << std::endl;
					std::cout << "p2 health : " << p2.getHealth() << std::endl;
					shots_it = shots.erase(shots_it);

				} else {

					shots_it++;
					 
				}
			}

			/////////////////////////////////////////////////////
			//	GO BACK TO MAIN MENU
			/////////////////////////////////////////////////////

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				return SCREENS::menu;


			window.clear();
			background.drawBG(window);
			p1.drawShip(window);
			p2.drawShip(window);

			for (Shot &shot : shots)
				window.draw(shot.getBullet());

			window.display();

			/////////////////////////////////////////////////////
			//	Checking for the death of a player
			/////////////////////////////////////////////////////

			if (p1.getHealth() <= 0) {
				
				if (explosionSound.getStatus() != Sound::Playing)
					explosionSound.play();

				endGame.setWinner(p2);
				p1.dropDead(window);

				while (explosionSound.getStatus() == Sound::Stopped) {
					return SCREENS::endGame;
				}
			} else if (p2.getHealth() <= 0) {
				if (explosionSound.getStatus() != Sound::Playing)
					explosionSound.play();

				endGame.setWinner(p1);
				p2.dropDead(window);

			while (explosionSound.getStatus() != Sound::Playing) {
				return SCREENS::endGame;
			}
		}
	}
	return 0;
}



