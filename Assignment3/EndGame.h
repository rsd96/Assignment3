#pragma once
#include "Screens.h"
#include "Ship.h"

class EndGame : public Screens {
public:
	EndGame();
	~EndGame();

	int Run(sf::RenderWindow& window);
	void setWinner(Ship winner) { m_winner = winner; };


private:
	sf::Texture m_bg;
	sf::Sprite m_endBG;
	sf::Text m_endText;
	sf::Text m_restartText;
	sf::Text m_mainMenuText;
	sf::Font m_font;
	static Ship m_winner;


};

