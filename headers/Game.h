#pragma once
#include "AlienPlayer.h"
#include "Labyrinth.h"
#include "printHelperMethods.h"

class Game
{
private:
	Labyrinth m_labyrinth;
	GAME_MODE m_gameMode;
public:
	Game()
	{
		// TODO
		// Here should be generated labyrinth
		// Generate new maze while its not valid

		/// Here user choose _game_mode
		gameModeSetter();

		/// Clearing menu to start the gameplay
		system("cls");

		/// Generating labyrinth based on game mode
		m_labyrinth = Labyrinth(m_gameMode);
	}

	/// <summary>
	/// Runs game's logic
	/// </summary>
	void play()
	{
		// TODO
		// Implement here game logic
		while (true)
		{
			m_labyrinth.printBoard();
			bool isPlayerMoved;
			char press = _getche();
			switch (press)
			{
			case 'w':
				isPlayerMoved = m_labyrinth.movePlayer(UP);
				break;
			case 'd':
				isPlayerMoved = m_labyrinth.movePlayer(RIGHT);
				break;
			case 's':
				isPlayerMoved = m_labyrinth.movePlayer(DOWN);
				break;
			case 'a':
				isPlayerMoved = m_labyrinth.movePlayer(LEFT);
				break;
			}
			if(isPlayerMoved)
				m_labyrinth.moveEnemies(m_gameMode);
			system("cls");	
		}
	}

	/// <summary>
	/// Temprorary function, should be removed in the end
	/// </summary>
	/// <returns></returns>
	Labyrinth getLabyrinth()
	{
		return m_labyrinth;
	}
private:
	void stop()
	{
		// TODO
		// This function should be called in play(), if player loses. Should print GAME OVER, and stop all
		// processes in play() 
	}

	/// <summary>
	/// Showing solution of labyrinth
	/// </summary>
	void show_solution()
	{
		// TODO
		// Implement function, which prints winning strategy from starting cell
		// This function should be called in stop(), if player wants
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