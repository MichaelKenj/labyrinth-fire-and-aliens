#pragma once
#include "AbstractLabyrinth.h"
#include "FireLabyrinth.h"
#include "AlienLabyrinth.h"
#include "printHelperMethods.h"
#include "Game_Mode.h"

class Game
{
private:
	void createLabyrinth() 
	{
		if (m_gameMode == FIRE) 
			m_labyrinth = new FireLabyrinth();
		else if (m_gameMode == ALIEN) 
			m_labyrinth = new AlienLabyrinth();
	}
private:
	/// TODO
	/// Change to smart pointer later
	AbstractLabyrinth* m_labyrinth;
	GAME_MODE m_gameMode;
public:
	Game()
	{
		gameModeSetter();
		createLabyrinth();
		system("cls");
	}

	~Game() 
	{
		delete m_labyrinth;
	}

	// Runs game's logic
	void play()
	{
		while (!m_labyrinth->isPlayerCaughtByEnemy() && !m_labyrinth->isMazeSolved())
		{
			m_labyrinth -> printBoard();
			bool isPlayerMoved = false;
			char press = _getche();
			
			switch (press)
			{
			case 'w':
				isPlayerMoved = m_labyrinth->movePlayer(UP);
				break;
			case 'd':
				isPlayerMoved = m_labyrinth->movePlayer(RIGHT);
				break;
			case 's':
				isPlayerMoved = m_labyrinth->movePlayer(DOWN);
				break;
			case 'a':
				isPlayerMoved = m_labyrinth->movePlayer(LEFT);
				break;
			}
			if(isPlayerMoved)
				m_labyrinth->moveEnemies();
			ClearWindow();
		}

		if (!m_labyrinth->isPlayerCaughtByEnemy())
		{
			win();
		}
		else
		{
			lose();
			stop();
		}
	}
private:
	void stop()
	{
		toSeeTheSolution();
	}

	void win()
	{
		slideWin();
	}

	void lose()
	{
		slideGameOver();
	}

	void toSeeTheSolution()
	{
		do
		{
			char toSeeOp;
			printIsPlayerWantsToShowSolutionFrame();
			toSeeOp = _getche();
			if (toSeeOp == '1')
			{
				showSolution();
				break;
			}
			else if (toSeeOp == '2')
				break;
		} while (true);
	}

	// Sets game mode and printing menu
	void gameModeSetter()
	{
		char main_menu_op;
		bool isInnerLoopBreaked = false;
		do
		{
			if (isInnerLoopBreaked)
				break;

			printMainMenuFrame();

			main_menu_op = _getche();
			if (main_menu_op == '1')
			{
				char choose_mode_op;
				do
				{
					printChooseGameModeFrame();

					choose_mode_op = _getche();
					if (choose_mode_op == '1')
					{
						m_gameMode = FIRE;
						isInnerLoopBreaked = true;
						break;
					}
					else if (choose_mode_op == '2')
					{
						m_gameMode = ALIEN;
						isInnerLoopBreaked = true;
						break;
					}
					else if (choose_mode_op == '3')
					{
						break;
					}
				} while (true);
			}
			else if (main_menu_op == '2')
				exit(0);
		} while (true);
	}

	// Animating solution of labyrinth
	void showSolution()
	{
		std::size_t index = 1;

		// Restoring initial data of labyrinth
		auto winningPath = m_labyrinth->getWinningPath();
		m_labyrinth->setPlayerPosition(m_labyrinth->getEntrance());
		m_labyrinth->restoreEnemy();
		m_labyrinth->restoreBoard();
		system("cls");
		while (!m_labyrinth->isPlayerCaughtByEnemy() && !m_labyrinth->isMazeSolved())
		{
			m_labyrinth->printBoard();
			bool isPlayerMoved = false;
			isPlayerMoved = m_labyrinth->movePlayer(winningPath[index]);
			if (isPlayerMoved)
			{
				m_labyrinth->moveEnemies();
				++index;
			}
			Sleep(90);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		}
	}
};