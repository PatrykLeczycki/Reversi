#include "Computer.h"

void Computer::enterField(Board &b)
{
	while (true)
	{
		int row = rand() % SIZEBOARD;
		int column = rand() % SIZEBOARD;

		if (isMoveCorrect(row, column, b))
		{
			Sleep(200);
			break;
		}
	}
}