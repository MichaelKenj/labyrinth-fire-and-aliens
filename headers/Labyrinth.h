#pragma once
#include <vector>
#include "HumanPlayer.h"
#include "Game_Mode.h"

using Board = std::vector<std::vector<char>>;

class Labyrinth
{
private:
	Board _board;
	Human_Player _player;
	// TODO
	// How to interact with fires or aliens? Should we use vector of enemies(Should we to use template for Labyrinth) 
	// or smth like that?
	// For example, after player's move to another cell, should be called function spread() for all fire or move() 
	// for all aliens
public:
	Labyrinth() = default;

	Labyrinth(GAME_MODE flag)
	{
		generate_board();
		if (flag == FIRE)
		{
			generate_fire();
		}
		else
		{
			generate_alien();
		}
	}

	void generate_board()
	{
		// TODO
		// Implement function, which generates labyrinth and set entrance and exit cell
	}

	//---------------FIRE----------------
	void generate_fire()
	{
		// TODO
		// Implement function, which choose randomly fire's count and put it into board
	}

	void spread_fire()
	{
		// TODO
		// Implement function, which spread fire to empty cells
	}
	//---------------FIRE----------------

	//---------------ALIEN----------------
	void generate_alien()
	{
		// TODO
		// Implement function, which choose randomly alien's count and put it into board
	}
	//---------------ALIEN----------------

	bool isValid() const
	{
		// if isWinable, isSolvableIn5Moves -> true
	}

	bool isWinable() const
	{

	}

	bool isSolvableIn5Moves() const
	{

	}



};

