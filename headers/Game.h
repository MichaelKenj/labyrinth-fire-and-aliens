#pragma once
#include "AlienPlayer.h"
#include "Labyrinth.h"
#include "printHelperMethods.h"

class Game
{
private:
	Labyrinth _labyrinth;
	GAME_MODE _game_mode;
public:
	Game()
	{
		// TODO
		// Here should be generated labyrinth

		/// Here user choosing _game_mode
		printInterface(_game_mode);
	}


	void play()
	{
		// TODO
		// Implement here game logic
	}

	
private:
	void stop()
	{
		// TODO
		// This function should be called in play(), if player loses. Should print GAME OVER, and stop all
		// processes in play() 
	}

	void show_solution()
	{
		// TODO
		// Implement function, which prints winning strategy from starting cell
	}

};