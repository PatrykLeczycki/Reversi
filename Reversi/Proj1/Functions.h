#pragma once
#include "Board.h"
#include "Player.h"
#include <string>
#include <windows.h>

void Board::countCounters(Player* p) const
{
	int sc = 0;
	for (int i = 0; i < SIZEBOARD; i++)
		for (int j = 0; j < SIZEBOARD; j++)
			if (m_playableBoard[i][j] == p->getCounter()) sc++;
	p->setScore(sc);
}

void colorText(const WORD color, string text)
{
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD index = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), color);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	SetConsoleTextAttribute(hstdout, color);
	cout << text;

	FlushConsoleInputBuffer(hstdin);
	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

void info1()
{
	centerstring("REVERSI");
	cout << endl;
	centerstring("Semestral project PO 2016/2017");
	cout << endl;
	centerstring("Author: Patryk Leczycki");
	cout << endl << endl;
}

void info2()
{
	centerstring("MENU\n\n");
	centerstring("1 - Play Reversi\n");
	centerstring("2 - Enter highscore\n");
	centerstring("3 - Load game\n");
	centerstring("4 - End game\n\n");
}

void info3()
{
	centerstring("Choose your mode:  ");
	cout << endl << endl;
	centerstring("1. Player vs player");
	cout << endl;
	centerstring("2. Player vs AI");
	cout << endl;
	centerstring("3. AI vs AI");
	cout << endl << endl;
}

void info4()
{
	cout << endl;
	centerstring("Game starts in 3...\n");
	Sleep(1000);
	centerstring("2...\n");
	Sleep(1000);
	centerstring("1...");
	Sleep(1000);
	system("cls");
}

struct score
{
	string date;
	string nick;
	int points;
	int length;
	score *next;
	score *prev;

	score(const string&, const string&, int, int);
};

int compare(const void* sc1, const void *sc2)
{
	score *p1 = *((score**)sc1);
	score *p2 = *((score**)sc2);
	if (p1->points > p2->points) return -1;
	if (p1->points == p2->points) return 0;
	if (p1->points < p2->points) return 1;
}

score::score(const string& _date, const string& _nick, int _points, int _length)
	: date(_date), nick(_nick), points(_points), length(_length), next(nullptr), prev(nullptr)
{ }

class Scores
{
	double averageScore;
	double averageTime;
	score *head, *tail;
	unsigned nodes;
public:
	Scores();
	~Scores();
	void add(string, string, int, int);
	void sortAndPrint();

};


Scores::Scores() : head(nullptr), tail(nullptr), averageScore(0), averageTime(0), nodes(0)
{
	fstream h;
	h.open("highscore.txt", std::ios::in);
	if (h.good())
	{
		int iter = 0;
		while (true)
		{
			string tempDate1, tempDate2, tempNick;
			double tempPoints, tempLength;

			while (h >> tempDate1 && h >> tempDate2 && h >> tempNick && h >> tempPoints && h >> tempLength)
			{
				string tempDate = tempDate1 + " " + tempDate2;

				add(tempDate, tempNick, tempPoints, tempLength);

				averageScore += tempPoints;
				averageTime += tempLength;
				iter++;
			}

			if (!iter)
				centerstring("Highscore is empty.\n\n");
			else
			{
				cout << endl << "Date Nickname Points Game length (s)" << endl << endl;
				sortAndPrint();
			}
			break;
		}
	}
	else
	{
		centerstring("Highscore is empty.");
		h.open("Highscore.txt", std::ios::out);
	}
	h.close();
	system("attrib +R highscore.txt");
}

Scores::~Scores()
{
	score *toDelete;

	while (head)
	{
		toDelete = head;
		head = head->next;
		delete toDelete;
	}
};

void Scores::add(string date, string nick, int points, int length)
{
	score *node = new score(date, nick, points, length);
	if (head == nullptr)
		head = tail = node;

	else
	{
		tail->next = node;
		node->prev = tail;
		tail = tail->next;
	}
	nodes++;
};


void Scores::sortAndPrint()
{
	score **pointersToNodes = new score*[nodes];
	score *temp = head;
	int i = 0;
	while (temp)
	{
		pointersToNodes[i] = temp;
		temp = temp->next;
		i++;
	}

	qsort(pointersToNodes, nodes, sizeof(score*), compare);

	for (int j = 0; j < nodes; j++)
		cout << pointersToNodes[j]->date << " " << pointersToNodes[j]->nick << " " << pointersToNodes[j]->points << " " << pointersToNodes[j]->length << endl;
	if (i)
	{
		cout << endl;
		centerstring("Average score: ");
		cout << averageScore / (double)nodes << endl;
		centerstring("Average time: ");
		cout << averageTime / (double)nodes << endl << endl;
	}
	delete[] pointersToNodes;
}
