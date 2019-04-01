#include "Board.h"
#include "Functions.h"
#include <fstream>
#include "Header.h"

Board::Board()
{
	for (int i = 0; i < SIZEBOARD; i++)
		for (int j = 0; j < SIZEBOARD; j++)
			m_playableBoard[i][j] = ' ';
	m_playableBoard[(SIZEBOARD / 2) - 1][(SIZEBOARD / 2) - 1] = m_playableBoard[SIZEBOARD / 2][SIZEBOARD / 2] = 'X';
	m_playableBoard[(SIZEBOARD / 2) - 1][SIZEBOARD / 2] = m_playableBoard[SIZEBOARD / 2][(SIZEBOARD / 2) - 1] = 'O';
}

void Board::draw() const
{
	char alpha[5] = "    ";
	char num[5] = "    ";
	char horizontalLine[5] = " ___";
	char verticalLine[5] = "|   ";
	char horizontalAndVerticalLine[5] = "|___";

	cout << alpha;
	alpha[2] = 'A';
	num[2] = '1';
	for (int i = 0; i < SIZEBOARD; i++)
	{
		cout << alpha;
		alpha[2]++;
	}
	cout << endl;

	for (int i = 0; i < SIZEBOARD; i++)
	{
		if (!i % 3)
		{
			cout << "    ";
			for (int j = 0; j < SIZEBOARD; j++) cout << horizontalLine;
			cout << endl;
		}
		for (int j = 0; j < SIZEBOARD; j++)
		{
			if (!j)
			{
				cout << num;
				num[2]++;
			}
			verticalLine[2] = m_playableBoard[i][j];
			if (i == m_lastMove[0] && j == m_lastMove[1])
			{
				cout << verticalLine[0] << verticalLine[1];
			
				char temp[2];
				temp[0] = verticalLine[2];
				temp[1] = '\0';
				string temp1(temp);
				colorText(0x0c, temp1);
				cout << verticalLine[3];
			}
			else cout << verticalLine;
		}
		cout << "|" << endl << "    ";
		for (int j = 0; j < SIZEBOARD; j++) cout << horizontalAndVerticalLine;
		cout << "|" << endl;
	}
	cout << endl;
};

void Board::draw(fstream &output) const
{
	char alpha[5] = "    ";
	char num[5] = "    ";
	char horizontalLine[5] = " ___";
	char verticalLine[5] = "|   ";
	char horizontalAndVerticalLine[5] = "|___";

	output << alpha;
	alpha[2] = 'A';
	num[2] = '1';
	for (int i = 0; i < SIZEBOARD; i++)
	{
		output << alpha;
		alpha[2]++;
	}
	output << endl;

	for (int i = 0; i < SIZEBOARD; i++)
	{
		if (!i % 3)
		{
			output << "    ";
			for (int j = 0; j < SIZEBOARD; j++) output << horizontalLine;
			output << endl;
		}
		for (int j = 0; j < SIZEBOARD; j++)
		{
			if (!j)
			{
				output << num;
				num[2]++;
			}
			verticalLine[2] = m_playableBoard[i][j];
			output << verticalLine;
		}
		output << "|" << endl << "    ";
		for (int j = 0; j < SIZEBOARD; j++) output << horizontalAndVerticalLine;
		output << "|" << endl;
	}
	output << endl;
}

// out - poprawnie otwarte
void Board::saveBoard(fstream& out) const
{
	for (int i = 0; i < SIZEBOARD; i++)
	{
		for (int j = 0; j < SIZEBOARD; j++)
			out << m_playableBoard[i][j];
		out << endl;
	}
}

void Board::loadBoard(char other[SIZEBOARD][SIZEBOARD])
{
	for (int i = 0; i < SIZEBOARD; i++)
		for (int j = 0; j < SIZEBOARD; j++)
			m_playableBoard[i][j] = other[i][j];
}