#pragma once
#include "AbstractLabyrinth.h"
#include "FireLabyrinth.h"
#include "AlienLabyrinth.h"
#include "printHelperMethods.h"
#include "Game_Mode.h"

class Game
{
private:
	std::unique_ptr<AbstractLabyrinth> m_labyrinth;
	GameMode m_gameMode;

	void createLabyrinth()
	{
		if (m_gameMode == GameMode::eFire)
			m_labyrinth = std::make_unique<FireLabyrinth>();
		else if (m_gameMode == GameMode::eAlien)
			m_labyrinth = std::make_unique<AlienLabyrinth>();
	}

public:
	Game()
	{
		gameModeSetter();
		createLabyrinth();
		system("cls");
	}

	~Game() = default;

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
				isPlayerMoved = m_labyrinth->movePlayer(Direction::eUp);
				break;
			case 'd':
				isPlayerMoved = m_labyrinth->movePlayer(Direction::eRight);
				break;
			case 's':
				isPlayerMoved = m_labyrinth->movePlayer(Direction::eDown);
				break;
			case 'a':
				isPlayerMoved = m_labyrinth->movePlayer(Direction::eLeft);
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
						m_gameMode = GameMode::eFire;
						isInnerLoopBreaked = true;
						break;
					}
					else if (choose_mode_op == '2')
					{
						m_gameMode = GameMode::eAlien;
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