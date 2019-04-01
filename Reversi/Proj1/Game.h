#pragma once
#define trace(x) cout << #x << " " << x << endl;

#include <time.h>

#include "Board.h"
#include "Player.h"



class Game
{
	// member variables
	Board m_board;
	Player *m_player[2];
	Player *m_currentPlayer;
	int m_modeMenu;
	char m_hintMenu;

	// private methods

	void createPlayers();
	void proceedGame();
	void gameOver(int, time_t, time_t) const;

protected:
	// saving and loading methods
	void save(Player*) const;
	bool loadGame();

	// other methods
	char *currentTime() const;
	bool isCounterValid(char field) const { return field == 'X' || field == 'O' || field == ' '; }

public:
	// constructors and destructors
	Game();
	~Game() { delete[] m_player; }
};