#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <limits>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::fstream;

const int SIZEBOARD = 8;

void centerstring(const string& s)
{
	int l = s.length();
	int pos = (int)((80 - l) / 2);
	string to_return;
	for (int i = 0; i < pos; i++) cout << " ";
	cout << s;
}

template <typename T>
T option(bool center, const string& statement, T min, T max, bool range)
{
	using namespace std;
	T to_return;
	while (true)
	{
		if (center) centerstring(statement);
		else cout << statement;

		cin >> to_return;
		to_return = toupper(to_return);

		if (cin.get() != '\n')
		{
			cout << endl;
			cin.clear();
			while (cin.get() != '\n');
		}
		else if (range)
		{
			if (to_return >= min && to_return <= max)
				break;
		}
		else if (to_return == min || to_return == max)
			break;

		centerstring("Wrong input.\n");
	}
	cout << endl;
	return to_return;
}

template <typename T>
T option(bool center, const string& statement, T min, T max, bool range, char saveOption)
{
	saveOption = toupper(saveOption);
	T to_return;
	while (true)
	{
		if (center) centerstring(statement);
		else cout << statement;

		cin >> to_return;
		to_return = toupper(to_return);

		if (cin.get() != '\n')
		{
			cout << endl;
			while (cin.get() != '\n');
		}
		else if (range)
		{
			if ((to_return >= min && to_return <= max) || to_return == saveOption )
				break;
		}
		else if (to_return == min || to_return == max)
			break;

		centerstring("Wrong input.\n");
	}
	cout << endl;
	return to_return;
}