#pragma once

#include "Header.h"
#include <fstream>

class Player;

class Board
{
public:
	// member variables
	char m_playableBoard[SIZEBOARD][SIZEBOARD];
	int m_lastMove[2];

	// constructors and destructors
	Board();

	// drawing, saving and loading methods
	void draw() const;
	void draw(fstream&) const;
	void saveBoard(fstream&) const;
	void loadBoard(char[SIZEBOARD][SIZEBOARD]);

	// other methods
	bool isEmpty(int row, int column) const { return m_playableBoard[row][column] == ' '; }
	void countCounters(Player*) const;
};


