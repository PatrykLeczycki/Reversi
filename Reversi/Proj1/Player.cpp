
#include "Header.h"
#include "Player.h"
#include "Board.h"


int Player::m_turnNumber = 0;

bool Player::isMoveCorrect(const int tempRow, const int tempColumn, Board& board)
{
	if (m_possibleFields[tempRow][tempColumn])
	{
		board.m_playableBoard[tempRow][tempColumn] = m_counter;
		wololo(tempRow, tempColumn, board);
		m_turnNumber++;
		board.m_lastMove[0] = tempRow;
		board.m_lastMove[1] = tempColumn;
		return true;
	}
	return false;
}

void Player::setFlag(const int x, const int y, int& possibleMovesNumber)
{
	if (!m_possibleFields[x][y])
	{
		m_possibleFields[x][y] = true;
		possibleMovesNumber++;
	}
}

void Player::checkLine(const Board &board, int &possibleMovesNumber, const int r, const int c, bool &playerFlag1, bool &playerFlag2, bool &opponentFlag1, bool &opponentFlag2)
{
	const char opponentCounter = (m_counter == 'O' ? 'X' : 'O');

	if (board.m_playableBoard[r][c] == m_counter)
		playerFlag1 = true;

	if (playerFlag1 && board.m_playableBoard[r][c] == opponentCounter)
		opponentFlag1 = true;

	if (playerFlag1 && !opponentFlag1 && board.m_playableBoard[r][c] == ' ')
		playerFlag1 = opponentFlag1 = false;

	if (playerFlag1 && opponentFlag1 && board.m_playableBoard[r][c] == m_counter)
		opponentFlag1 = false;

	if (playerFlag1 && opponentFlag1 && board.m_playableBoard[r][c] == ' ')
	{
		setFlag(r, c, possibleMovesNumber);
		playerFlag1 = opponentFlag1 = false;
	}

	/////////////////////////////////

	if (board.m_playableBoard[c][r] == m_counter)
		playerFlag2 = true;

	if (playerFlag2 && board.m_playableBoard[c][r] == opponentCounter)
		opponentFlag2 = true;

	if (playerFlag2 && !opponentFlag2 && board.m_playableBoard[c][r] == ' ')
		playerFlag2 = opponentFlag2 = false;

	if (playerFlag2 && opponentFlag2 && board.m_playableBoard[c][r] == m_counter)
		opponentFlag2 = false;

	if (playerFlag2 && opponentFlag2 && board.m_playableBoard[c][r] == ' ')
	{
		setFlag(c, r, possibleMovesNumber);
		playerFlag2 = opponentFlag2 = false;
	}
}

void Player::mayBePlaced(const Board &board, const int index, int moves[])
{
	for (int i = 0; i < SIZEBOARD; i++)
		for (int j = 0; j < SIZEBOARD; j++)
			m_possibleFields[i][j] = false;

	bool playerCounterFound1 = false;
	bool playerCounterFound2 = false;
	bool opponentCounterFound1 = false;
	bool opponentCounterFound2 = false;
	int count = 0;

	// Horizontal and vertical checking

	for (int i = 0; i < SIZEBOARD; i++)
	{
		for (int j = 0; j < SIZEBOARD; j++)
			checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);

		playerCounterFound1 = playerCounterFound2 = opponentCounterFound1 = opponentCounterFound2 = false;
	}

	// Backward horizontal and vertical checking

	for (int i = 0; i < SIZEBOARD; i++)
	{
		for (int j = SIZEBOARD - 1; j >= 0; j--)
			checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);

		playerCounterFound1 = playerCounterFound2 = opponentCounterFound1 = opponentCounterFound2 = false;
	}

	// Diagonal checking

	for (int x = SIZEBOARD - 3; x >= 0; x--)
	{
		for (int i = x, j = 0; i < SIZEBOARD && j < SIZEBOARD; i++, j++)
			checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);

		playerCounterFound1 = opponentCounterFound1 = playerCounterFound2 = opponentCounterFound2 = false;
	}

	for (int x = 2; x < SIZEBOARD; x++)
	{
		for (int i = SIZEBOARD - 1, j = x; i >= 0 && j >= 0; i--, j--)
			checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);

		playerCounterFound1 = opponentCounterFound1 = playerCounterFound2 = opponentCounterFound2 = false;
	}

	/////////////////////////////////////////////////////////////////////////////////

	{
		int temp1 = 2;
		int i = temp1, j = 0;

		do
		{
			for (i = temp1++, j = 0; i >= 0 && i < SIZEBOARD && j >= 0 && j < SIZEBOARD; i--, j++)
			{
				checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);
				if (!i && j == SIZEBOARD - 1) break;
			}
			playerCounterFound1 = opponentCounterFound1 = playerCounterFound2 = opponentCounterFound2 = false;
		} while (i || j != SIZEBOARD - 1);
	}

	/////////////////////////////////////////////////////////////////////////////////

	{
		int temp = 5;
		int i = 7, j = temp;

		do
		{
			for (i = 7, j = temp--; i >= 0 && i < SIZEBOARD && j >= 0 && j < SIZEBOARD; i--, j++)
			{
				checkLine(board, count, i, j, playerCounterFound1, playerCounterFound2, opponentCounterFound1, opponentCounterFound2);
				if (!i && j == SIZEBOARD - 1) break;
			}
			playerCounterFound1 = opponentCounterFound1 = playerCounterFound2 = opponentCounterFound2 = false;
		} while (i || j != SIZEBOARD - 1);
	}
	moves[index] = count;
}

void Player::enterField(Board &b)
{
	bool isFieldEmpty = false;
	int temp = 0;	// if temp>0 then program prints info about wrong input data
	do
	{
		if (temp) cout << "Wrong field. Try again." << endl << endl;

		m_column = option(false, "Enter column (A-H) or save the game (S):  ", 'A', 'H', true, 'S');

		if (saveGame())
		{
			cout << endl << "Saving game." << endl;
			break;
		}

		m_row = option<int>(false, "Enter row (1-8):  ", 1, 8, true);

		m_column -= 'A';
		m_row--;

		if (b.m_playableBoard[m_row][m_column] == 'X' || b.m_playableBoard[m_row][m_column] == 'O')
		{
			cout << "Chosen field is taken. Try again." << endl << endl;
			continue;
		}
		else isFieldEmpty = true;
		temp++;
	} while (!isFieldEmpty || !isMoveCorrect(m_row, m_column, b));
}

void Player::isWololoPossible(Board &board, const int row, const int column, bool &opponentFlag, bool &wrongWay, bool &wololoConfirmed)
{
	const char opponentCounter = (m_counter == 'O' ? 'X' : 'O');

	if (board.m_playableBoard[row][column] == opponentCounter)
		opponentFlag = true;

	if ((!opponentFlag && board.m_playableBoard[row][column] == m_counter) || board.m_playableBoard[row][column] == ' ')
		wrongWay = true;

	if (opponentFlag && board.m_playableBoard[row][column] == m_counter)
		wololoConfirmed = true;
}

void Player::wololo(const int row, const int column, Board &board)
{

	int x, y;

	bool opponentCounterFound = false;
	bool wololoConfirmed = false;
	bool wrongWay = false;
	int numberOfWololo = 0;


	// HORIZONTAL

	for (x = column + 1; x < SIZEBOARD; x++)
	{
		isWololoPossible(board, row, x, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x - 1; i > column; i--)
			{
				board.m_playableBoard[row][i] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	for (x = column - 1; x >= 0; x--)
	{
		isWololoPossible(board, row, x, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x + 1; i < column; i++)
			{
				board.m_playableBoard[row][i] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	// VERTICAL

	for (x = row + 1; x < SIZEBOARD; x++)
	{
		isWololoPossible(board, x, column, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x - 1; i > row; i--)
			{
				board.m_playableBoard[i][column] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	for (x = row - 1; x >= 0; x--)
	{
		isWololoPossible(board, x, column, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x + 1; i < row; i++)
			{
				board.m_playableBoard[i][column] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	// DIAGONAL

	for (x = row + 1, y = column + 1; x < SIZEBOARD && y < SIZEBOARD; x++, y++)
	{
		isWololoPossible(board, x, y, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x - 1, j = y - 1; i > row && y > column; i--, j--)
			{
				board.m_playableBoard[i][j] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	for (x = row - 1, y = column - 1; x >= 0 && y >= 0; x--, y--)
	{
		isWololoPossible(board, x, y, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x + 1, j = y + 1; i < row && y < column; i++, j++)
			{
				board.m_playableBoard[i][j] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	for (x = row + 1, y = column - 1; x < SIZEBOARD && y >= 0; x++, y--)
	{
		isWololoPossible(board, x, y, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x - 1, j = y + 1; i > row && y < column; i--, j++)
			{
				board.m_playableBoard[i][j] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}

	opponentCounterFound = wrongWay = wololoConfirmed = false;

	for (x = row - 1, y = column + 1; x >= 0 && y < SIZEBOARD; x--, y++)
	{
		isWololoPossible(board, x, y, opponentCounterFound, wrongWay, wololoConfirmed);
		if (wrongWay)
			break;

		if (wololoConfirmed)
		{
			for (int i = x + 1, j = y - 1; i < row && y > column; i++, j--)
			{
				board.m_playableBoard[i][j] = m_counter;
				numberOfWololo++;
			}
			break;
		}
	}
	setStatement(numberOfWololo);
}

void Player::setStatement(int captures)
{
	m_captureStatement += m_nickname + " captured " + std::to_string(captures);
	if (captures > 1)
		m_captureStatement += " counters.";
	else m_captureStatement += " counter.";
}

void Player::askForHint(const int index, int moves[])
{
	char hintChoice = option<char>(false, "Do you want to take a hint? (Y/N): ", 'N', 'Y', false);

	if (hintChoice == 'Y')
	{
		int i = rand() % SIZEBOARD;	// losowanie dostêpnego dla gracza miejsca na jego pionek
		int j = rand() % SIZEBOARD;

		while (!m_possibleFields[i][j])	// losuje, dopóki nie wylosuje odpowiedniego miejsca
		{
			i = rand() % SIZEBOARD;
			j = rand() % SIZEBOARD;
		}
		char column = j + 'A';
		cout << endl << "You can set a counter on a field " << column << i + 1 << endl << endl;
	}
}