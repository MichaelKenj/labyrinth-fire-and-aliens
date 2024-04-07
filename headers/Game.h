#pragma once
#include "AbstractLabyrinth.h"
#include "FireLabyrinth.h"
#include "AlienLabyrinth.h"
#include "printHelperMethods.h"
#include "Game_Mode.h"

class Game
{
private:
	void createLabyrinth() {
		if (m_gameMode == FIRE) {
			m_labyrinth = new FireLabyrinth();
		}
		else if (m_gameMode == ALIEN) {
			m_labyrinth = new AlienLabyrinth();
		}
	}
private:
	// Change to smart pointer later
	AbstractLabyrinth* m_labyrinth;
	//Human_Player m_player;
	GAME_MODE m_gameMode;
public:
	Game()
	{
		// TODO
		// Generate new maze while its not valid

		/// Here user choose _game_mode
		gameModeSetter();
		
		/// Generating labyrinth based on game mode
		// Should be in while loop (!isValid())
		createLabyrinth();

		// Should be 
		// do while loop(!isSolvable){generate enemies};


		/// Clearing menu to start the gameplay
		ClearScreen();
	}

	Game(const Game& game)
	{
		m_labyrinth = game.m_labyrinth;
		m_gameMode = game.m_gameMode;
	}

	~Game() {
		delete m_labyrinth; // Ensure proper cleanup
	}

	/// <summary>
	/// Runs game's logic
	/// </summary>
	void play()
	{
		
		//auto inter = m_labyrinth->findIntersectionCoordinate(m_labyrinth->getWinPath());
		//std::vector<Coordinate> farthestCoors;
		//for (auto i : inter)
		//{
		//	auto farthest = m_labyrinth->findFarthestEmptyCell(i);
		//	farthestCoors.push_back(farthest);
		//}

		while (!m_labyrinth -> isPlayerCaughtByEnemy() && !m_labyrinth -> isMazeSolved())
		{
			m_labyrinth -> printBoard();
			/*std::cout << "Winning path: ";
			for (auto i : m_labyrinth->getWinPath())
				printCoordinate(i);

			std::cout << '\n';
			
			for (int i = 0; i < inter.size(); ++i)
			{
				printCoordinate(inter[i]);
				std::cout << " -> ";
				printCoordinate(farthestCoors[i]);
				std::cout << "Distance(From Player): " << m_labyrinth->findPath(inter[i], m_labyrinth->getEntrance()).size() << " ";
				std::cout << "Distance(From Farthest): " << m_labyrinth->findPath(inter[i], farthestCoors[i]).size();
				std::cout << '\n';
			}*/

			bool isPlayerMoved = false;
			char press = _getche();
			
			switch (press)
			{
			case 'w':
				isPlayerMoved = m_labyrinth -> movePlayer(UP);
				break;
			case 'd':
				isPlayerMoved = m_labyrinth -> movePlayer(RIGHT);
				break;
			case 's':
				isPlayerMoved = m_labyrinth -> movePlayer(DOWN);
				break;
			case 'a':
				isPlayerMoved = m_labyrinth -> movePlayer(LEFT);
				break;
			}
			if(isPlayerMoved)
				m_labyrinth -> moveEnemies();
			ClearScreen();
		}

		if (!m_labyrinth ->isPlayerCaughtByEnemy())
		{
			//WIN
			win();
		}
		else
		{
			//LOSE
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

	/// <summary>
	/// Sets game mode and printing menu
	/// </summary>
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

	/// <summary>
	/// Showing solution of labyrinth
	/// </summary>
	void showSolution()
	{
		auto winningPath = m_labyrinth->getWinPath();
		std::size_t index = 0;

		m_labyrinth -> setPlayerPosition(m_labyrinth -> getEntrance());
		m_labyrinth -> restoreEnemy();
		m_labyrinth -> restoreBoard();
		while (!m_labyrinth -> isPlayerCaughtByEnemy() && !m_labyrinth -> isMazeSolved())
		{
			m_labyrinth -> printBoard();
			bool isPlayerMoved = false;
			isPlayerMoved = m_labyrinth->movePlayer(winningPath[index]);
			if (isPlayerMoved)
			{
				m_labyrinth -> moveEnemies();
				++index;
			}
			Sleep(120);
			ClearScreen();
		}
	}
};