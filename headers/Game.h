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
		system("cls");
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
		
		
		/*m_labyrinth->printBoard();
		auto intersections = m_labyrinth->findIntersectionCoordinate(m_labyrinth->getWinPath());
		std::cout << "Winning paths: ";
		for (auto u : m_labyrinth->getWinPath())
		{
			for(auto j : u)
				printCoordinate(j);
			std::cout << "| \n";
		}
		for (auto u : intersections)
		{
			for (auto i : u)
			{
				auto coor = m_labyrinth -> findFarthestEmptyCell(i);
				std::cout << "\n(" << i.first << ";" << i.second << ") -> " <<
					"(" << coor.first << ";" << coor.second << ")";

			}
			std::cout << "\nSecond winning path---------\n";
		}
		*/
		
		// check is player alive in Player
		// Put this while into separate function later
		while (!m_labyrinth -> isPlayerCaughtByEnemy() && !m_labyrinth -> isMazeSolved())// here should be isAlive player()
		{
			m_labyrinth -> printBoard();
			 //move move logic into separate function
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
			system("cls");	
			

		}

		/// Indicate if the game is lost or won
		if (!m_labyrinth ->isPlayerCaughtByEnemy())
		{
			win();
			//std::cout << "WIN\n";
		}
		else
		{
			stop();
			//std::cout << "LOSE\n";
		}
	}

	/// <summary>
	/// Showing solution of labyrinth
	/// </summary>
	void showSolution()
	{
		// TODO
		// Implement function, which prints winning strategy from starting cell
		// This function should be called in stop(), if player wants
		auto winVec = solve();
		for (auto i : winVec)
		{
			printCoordinate(i);
		}
	}

private:
	// Game over print
	void stop()
	{
		// TODO
		// This function should be called in play(), if player loses. Should print GAME OVER, and stop all
		// processes in play() 
		slideGameOver();
		// should be called showSolution() if user wants 
	}

	void win()
	{
		slideWin();

	}
	std::vector<Coordinate> solve()
	{

		
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
};