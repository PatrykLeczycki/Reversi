#pragma once

#include "Player.h"

class Computer : public Player
{
public:
	// constructors and destructors
	Computer(int x, string name, bool human, Player *enemy) : Player(x, name, human, enemy) {}

	// methods
	void enterField(Board &);
};
