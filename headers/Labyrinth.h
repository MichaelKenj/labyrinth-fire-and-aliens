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
	std::vector < Coordinate > _fire_positions;
	std::vector< Coordinate > _alien_positions;
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
			generate_aliens();
		}
	}

	void generate_board()
	{
		// TODO
		// Implement function, which generates labyrinth and set entrance and exit cell
		//:(
	}

	//---------------FIRE----------------
	void generate_fire()
	{
		// TODO
		// Implement function, which chooses randomly fire's count and put it into board
	}

	void spread_fire()
	{
		// TODO
		// Implement function, which spreads fire to empty cells
	}
	//---------------FIRE----------------

	//---------------ALIEN----------------
	void generate_aliens()
	{
		// TODO
		// Implement function, which chooses randomly alien's count and put it into board
	}
	//---------------ALIEN----------------

	

	bool is_winable() const
	{

	}

	bool is_solvable_in_5_moves() const
	{

	}

	bool is_valid() const
	{
		if(is_winable() && is_solvable_in_5_moves())
		{
			return true;
		}
		return false;
	}

	bool is_player_on_fire() const 
	{
		return _board[_player.getPosition().first][_player.getPosition().second] == '@';
	}

	bool is_player_caught_by_alien() const 
	{
		return std::find(_alien_positions.begin(), _alien_positions.end(), _player.getPosition()) != _alien_positions.end();
	}

};

