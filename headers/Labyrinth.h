#pragma once
#include <vector>
#include "HumanPlayer.h"
#include "Game_Mode.h"

using Board = std::vector<std::vector<char>>;

class Labyrinth
{
private:
	Board _board;
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
		//zut labirintiky ira paterov u tashaxustov
	}

	//---------------FIRE----------------
	void generate_fire()
	{

	}

	void spread_fire()
	{

	}
	//---------------FIRE----------------

	//---------------ALIEN----------------
	void generate_alien()
	{

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

