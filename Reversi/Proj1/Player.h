#pragma once
#define trace(x) cout << #x << " " << x << endl;
#include "Board.h"

class Player
{
	// member variables
	Player *m_next;
	string m_nickname;
	int m_row;
	int m_column;
	char m_counter;
	int m_score;
	bool m_isHuman;
	static int m_turnNumber;
	bool m_possibleFields[SIZEBOARD][SIZEBOARD];
	string m_captureStatement;

	// methods
	void setFlag(const int, const int, int&);
	void checkLine(const Board &, int&, const int, const int, bool&, bool&, bool&, bool&);

protected:
	// methods
	bool isMoveCorrect(const int, const int, Board&);
	void mayBePlaced(const Board&, const int, int[]); //
	virtual void enterField(Board&);
	void wololo(const int, const int, Board&); //
	void isWololoPossible(Board&, int, int, bool&, bool&, bool&);
	bool saveGame() const { return m_column == 'S'; }
	void askForHint(const int, int[]);
	void clearStatement() { m_captureStatement.clear(); }

public:

	// constructors and destructors
	Player(int x, string name, bool human, Player *enemy) : m_counter(x % 2 ? 'X' : 'O'), m_nickname(name), m_isHuman(human), m_next(enemy) {}

	// Getters and setters
	int getRow() const { return m_row; }
	int getColumn() const { return m_column; }
	string getNickName() const { return m_nickname; }
	char getCounter() const { return m_counter; }
	bool isPlayerHuman() const { return m_isHuman; }
	int getScore() const { return m_score; }
	int getTurnNumber() const { return m_turnNumber; }
	Player* getEnemy() const { return m_next; }
	string getStatement() const { return m_captureStatement; }

	void setRow(int row) { m_row = row; }
	void setColumn(int column) { m_column = column; }
	void setEnemy(Player *enemy) { m_next = enemy; }
	void setScore(int x) { m_score = x; }
	void setNickname(string x) { m_nickname = x; }
	void setStatement(int);

	friend class Game;
};

