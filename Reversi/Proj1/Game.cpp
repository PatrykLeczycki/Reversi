#include "Game.h"
#include "Functions.h"
#include "Computer.h"

Game::Game()
	:m_modeMenu(0), m_hintMenu('A')
{
	info1();
	while (1)
	{
		info2();
		int menu = option<int>(true, "Your choice: ", 1, 4, true);
		if (menu == 4)
		{
			cout << endl;
			centerstring("End of program.\n\n");
			exit(0);
		}
		else if (menu == 3)
		{
			bool didItLoad = loadGame();
			Sleep(5000);
			system("cls");
			if (didItLoad)
			{
				info4();
				proceedGame();
			}
		}
		else if (menu == 2)
			Scores object;
		else
		{
			info3();
			m_modeMenu = option<int>(true, "Your choice: ", 1, 3, true);
			cout << endl;
			centerstring("Chosen mode: ");
			cout << m_modeMenu << endl << endl;
			createPlayers();
			if (m_modeMenu != 3)
			{
				cout << endl;
				m_hintMenu = option<char>(true, "Do you want to activate hint mode (Y/N)?: ", 'N', 'Y', false);
			}
			system("cls");
			info4();
			proceedGame();
		}
	}
}

char* Game::currentTime() const
{
	static char buffer[20];
	time_t actualTime;
	time(&actualTime);
	strftime(buffer, sizeof buffer, "%d-%m-%Y %T", localtime(&actualTime));
	return buffer;
}

void Game::gameOver(int x, time_t s, time_t f) const
{
	fstream highscore;
	highscore.open("highscore.txt", std::ios::app);

	switch (x)
	{
	case 1:
	{
		cout << endl << endl << "Game over. Both players can't move." << m_currentPlayer->getNickName() << " wins(" << m_currentPlayer->getScore() << ")." << endl << endl;
		highscore << currentTime() << " " << m_currentPlayer->getNickName() << " " << m_currentPlayer->getScore() << " " << difftime(f, s) << endl;
		exit(0);
	}
	case 2:
	{
		cout << endl << endl << "Game over. Both players can't move." << m_currentPlayer->getEnemy()->getNickName() << " wins(" << m_currentPlayer->getEnemy()->getScore() << ")." << endl << endl;
		highscore << currentTime() << " " << m_currentPlayer->getEnemy()->getNickName() << " " << m_currentPlayer->getEnemy()->getScore() << " " << difftime(f, s) << endl;
		exit(0);
	}
	case 3:
	{
		cout << endl << endl << "Game over. Both players can't move. Players tie." << endl << endl;
		string temp_nick = m_currentPlayer->getNickName() + "/" + m_currentPlayer->getEnemy()->getNickName();
		highscore << currentTime() << " " << temp_nick << " " << m_currentPlayer->getScore() << " " << difftime(f, s) << endl;
		exit(0);
	}
	case 4:
	{
		cout << m_currentPlayer->getNickName() << " has no counters left. " << m_currentPlayer->getEnemy()->getNickName() << " wins (" << m_currentPlayer->getEnemy()->getScore() << ")." << endl << endl;
		highscore << currentTime() << " " << m_currentPlayer->getEnemy()->getNickName() << " " << m_currentPlayer->getEnemy()->getScore() << " " << difftime(f, s) << endl;
		exit(0);
	}
	}
}

void Game::createPlayers()
{
	if (m_modeMenu == 1)
	{
		string nick1, nick2;

		cout << endl << endl;
		centerstring("Player A - enter your nickname: ");
		cin >> nick1;
		while (cin.get() != '\n');
		cout << endl << endl;
		m_player[0] = new Player(0, nick1, true, nullptr);

		centerstring("Player B - enter your nickname: ");
		cin >> nick2;
		while (cin.get() != '\n');
		while (nick2 == nick1)
		{
			centerstring("Players can't have the same nicks.\n\n");
			centerstring("Player B - enter your nickname: ");
			cin >> nick2;
			while (cin.get() != '\n');
		}

		m_player[1] = new Player(1, nick2, true, m_player[0]);
		m_player[0]->setEnemy(m_player[1]);

	}
	else if (m_modeMenu == 2)
	{
		string nick;

		cout << endl;
		centerstring("Player A - enter your nickname: ");
		cin >> nick;

		while (nick == "Computer")
		{
			centerstring("You can't have the same nick as Computer.\n\n");
			centerstring("Player B - enter your nickname: ");
			cin >> nick;
			while (cin.get() != '\n');
		}

		while (cin.get() != '\n');
		m_player[0] = new Player(0, nick, true, nullptr);
		m_player[1] = new Computer(1, "Computer", false, m_player[0]);
		m_player[0]->setEnemy(m_player[1]);
	}
	else
	{
		m_player[0] = new Computer(0, "Computer1", false, nullptr);
		m_player[1] = new Computer(1, "Computer2", false, m_player[0]);
		m_player[0]->setEnemy(m_player[1]);
	}
}

void Game::save(Player *p) const
{

	fstream save;
	string filename = "save_" + p->getNickName() + ".txt";
	save.open(filename, std::ios::out);
	save << p->getNickName() << endl << p->isPlayerHuman() << endl << p->getEnemy()->getNickName() << endl << p->getEnemy()->isPlayerHuman() << endl << m_modeMenu << endl << p->getTurnNumber() << endl;

	m_board.saveBoard(save);
	save.close();
	//string command = "attrib +R " + filename;
	//system(command.c_str());
}

bool Game::loadGame()
{
	fstream load;
	cout << endl;
	centerstring("Enter player's name to load the game: ");
	string tempNick;
	cin >> tempNick;
	cout << endl;
	string loadFilename = "save_" + tempNick + ".txt";

	bool isDataValid = true;

	load.open(loadFilename, std::ios::in);
	if (load.good())
	{
		int iter = 0;
		string nick1, nick2;
		string tempHuman1, tempHuman2;
		string temp1, temp2;
		char tempBoard[8][8];
		getline(load, nick1);
		getline(load, tempHuman1);
		getline(load, nick2);
		getline(load, tempHuman2);
		getline(load, temp1);
		getline(load, temp2);
		int mode = atoi(temp1.c_str());
		int move = atoi(temp2.c_str());
		int isHuman1 = atoi(tempHuman1.c_str());
		int isHuman2 = atoi(tempHuman2.c_str());

		if (nick1 != tempNick)
		{
			centerstring("Save file is corrupted (wrong nickname). Deleting save file.\n");
			isDataValid = false;
		}

		if (isHuman1 != 1 || ((isHuman2 != 1) && isHuman2))
		{
			centerstring("Save file is corrupted (wrong isHuman value). Deleting save file.\n");
			isDataValid = false;
		}

		if (mode > 3 || mode < 1)
		{
			centerstring("Save file is corrupted (wrong mode value). Deleting save file.\n");
			isDataValid = false;
		}

		if (move < 0)
		{
			centerstring("Save file is corrupted (wrong turn value). Deleting save file.\n");
			isDataValid = false;
		}
	
		if (isDataValid)
		{
			for (int i = 0; i < SIZEBOARD; i++)
			{
				for (int j = 0; j < SIZEBOARD; j++)
				{
					tempBoard[i][j] = load.get();
					if (!isCounterValid(tempBoard[i][j]))
					{
						centerstring("Save file is corrupted (wrong counter value). Deleting save file.\n");
						isDataValid = false;
						break;
					}
				}
				if (!isDataValid)
					break;
				load.get();
			}
		}

		if (isDataValid)
		{
			if (move % 2) // o save poprosil gracz 2
			{
				m_player[0] = (isHuman2 ? new Player(move, nick2, true, nullptr) : new Computer(move, nick2, false, nullptr));
				m_player[1] = (isHuman1 ? new Player(move + 1, nick1, true, m_player[0]) : new Computer(move, nick1, false, m_player[0]));
				m_player[0]->setEnemy(m_player[1]);
			}
			else  // o save poprosil gracz 1
			{
				m_player[0] = (isHuman1 ? new Player(move, nick1, true, nullptr) : new Computer(move, nick1, false, nullptr));
				m_player[1] = (isHuman2 ? new Player(move + 1, nick2, true, m_player[0]) : new Computer(move + 1, nick2, false, m_player[0]));
				m_player[0]->setEnemy(m_player[1]);
			}
			m_modeMenu = mode;
			m_board.loadBoard(tempBoard);
			centerstring("Game was successfully loaded.\n");
			return true;
		}
		else
		{
			load.close();
			/*fstream emptyIt;
			emptyIt.open(loadFilename, std::ios::out | std::ios::trunc);
			emptyIt.close();*/
			remove(loadFilename.c_str());
			return false;
		}
	}
	else
	{
		centerstring("Save file of player with given nickname doesn't exist.\n");
		return false;
	}
}

void Game::proceedGame()
{
	fstream outputForTests;
	outputForTests.open("output_test.txt", std::ios::out);
	if (!outputForTests.good())
	{
		cout << "Fatal error. End of program." << endl << endl;
		exit(0);
	}

	time_t start, finish;
	time(&start);
	m_currentPlayer = m_player[0];
	colorText(0x0c, "-----------------------------");
	outputForTests << "-----------------------------";
	cout << endl << "Turn: " << m_currentPlayer->getNickName() << "(" << m_currentPlayer->getCounter() << ")" << endl << endl;
	outputForTests << endl << "Turn: " << m_currentPlayer->getNickName() << "(" << m_currentPlayer->getCounter() << ")" << endl << endl;
	m_board.draw();
	int moves[2];
	int playerIndex = 0;

	while (1)
	{
		m_board.countCounters(m_currentPlayer);
		m_board.countCounters(m_currentPlayer->getEnemy());

		if (!m_currentPlayer->getScore())
		{
			time(&finish);
			gameOver(4, start, finish);
		}

		m_currentPlayer->mayBePlaced(m_board, playerIndex % 2, moves);

		if (moves[playerIndex % 2])
		{
			if (m_currentPlayer->isPlayerHuman() && m_hintMenu == 'Y')
				m_currentPlayer->askForHint(playerIndex % 2, moves);

			m_currentPlayer->enterField(m_board);
			if (m_currentPlayer->saveGame())
			{
				save(m_currentPlayer);
				cout << "Game saved successfully. End of program." << endl << endl;
				exit(0);
			}

			//system("cls");

			m_board.draw();
			m_board.draw(outputForTests);
			cout << endl;
			outputForTests << endl << "-----------------------------" << endl << m_currentPlayer->m_captureStatement << endl << "-----------------------------" << endl << "-----------------------------" << endl << endl;
			colorText(0x0b, "-----------------------------");
			cout << endl << m_currentPlayer->m_captureStatement << endl;
			colorText(0x0b, "-----------------------------");
			cout << endl;
			colorText(0x0c, "-----------------------------");
			cout << endl << endl;
			m_currentPlayer->clearStatement();
			m_board.countCounters(m_currentPlayer);
			playerIndex++;
		}
		else
		{
			playerIndex++;
			m_currentPlayer->getEnemy()->mayBePlaced(m_board, playerIndex % 2, moves);

			m_board.countCounters(m_currentPlayer);
			m_board.countCounters(m_currentPlayer->getEnemy());

			if (moves[playerIndex % 2])
			{
				cout << m_currentPlayer->getNickName() << " can't move and loses his turn." << endl;
				outputForTests << m_currentPlayer->getNickName() << " can't move and loses his turn." << endl << "-----------------------------" << endl;
				colorText(0x0b, "-----------------------------");
				cout << endl;
			}
			else if (m_currentPlayer->getScore() > m_currentPlayer->getEnemy()->getScore())
			{
				time(&finish);
				gameOver(1, start, finish);
			}
			else if (m_currentPlayer->getScore() < m_currentPlayer->getEnemy()->getScore())
			{
				time(&finish);
				gameOver(2, start, finish);
			}
			else
			{
				time(&finish);
				gameOver(3, start, finish);
			}
		}
		m_currentPlayer = m_currentPlayer->getEnemy();
		cout << "Turn: " << m_currentPlayer->getNickName() << "(" << m_currentPlayer->getCounter() << ")" << endl << endl;
		outputForTests << "Turn: " << m_currentPlayer->getNickName() << "(" << m_currentPlayer->getCounter() << ")" << endl << endl;
	}
}
