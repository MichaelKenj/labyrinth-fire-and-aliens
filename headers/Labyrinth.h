#pragma once
#include <vector>
#include "HumanPlayer.h"
#include "Game_Mode.h"
#include <iomanip>
#include <set>



using Board = std::vector<std::vector<char>>;

class Labyrinth
{
private:
	Board _board;
	Human_Player _player;
	std::size_t _size;
	std::vector < Coordinate > _fire_positions;
	std::vector< Coordinate > _alien_positions;
public:
	
	Labyrinth() = default;

	Labyrinth(GAME_MODE flag, std::size_t size = 20) : _size(size)
	{
		_board.assign(_size, std::vector<char>(_size, '#'));
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

	void print_board() const 
	{
		for (const auto& row : _board) 
		{
			for (char cell : row) 
				std::cout <<std::setw(2) << cell;
			std::cout << std::endl;
		}
	}

	std::vector<Coordinate> get_neighbouring_coordinates(Coordinate coor)
	{
		std::vector<Coordinate> res_vec;

		if (coor.first > 0)
			res_vec.push_back(Coordinate{ coor.first - 1, coor.second });

		if (coor.first < 19)
			res_vec.push_back(Coordinate{ coor.first + 1, coor.second });

		if (coor.second > 0)
			res_vec.push_back(Coordinate{ coor.first, coor.second - 1 });

		if (coor.second < 19)
			res_vec.push_back(Coordinate{ coor.first, coor.second + 1 });

		return res_vec;
	}
	//---------------FIRE----------------
	void generate_fire()
	{
		// TODO
		// Implement function, which chooses randomly fire's count and put it into board
		std::size_t fire_count;
		fire_count = generateRandomNumber(1, 3);
		std::cout << "\nFire count: " << fire_count << '\n';
		for (std::size_t i = 0; i < fire_count; ++i)
		{
			Coordinate new_coor;
			do
			{
				new_coor = generateRandomCoordinate(Coordinate{ 1,1 }, Coordinate{ _board.size() - 2, _board.size() - 2 });
			} while (is_wall(new_coor));
			_board[new_coor.first][new_coor.second] = '@';
			_fire_positions.push_back(new_coor);
		}

	}

	void spread_fire()
	{
		// TODO
	  	// Implement function, which spreads fire to empty cells
		std::vector<Coordinate> newFirePositions;

		for (const auto& firePosition : _fire_positions)
		{
			auto neighbors = get_neighbouring_coordinates(firePosition);
			for (const auto& neighbor : neighbors)
			{
				if (!is_wall(neighbor) && _board[neighbor.first][neighbor.second] != '@')
				{
					newFirePositions.push_back(neighbor);
					_board[neighbor.first][neighbor.second] = '@';
				}
			}
		}

		_fire_positions.insert(_fire_positions.end(), newFirePositions.begin(), newFirePositions.end());
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

	bool is_wall(Coordinate coor) const
	{
		return _board[coor.first][coor.second] == '#';
	}

	bool is_empty_space(Coordinate coor) const
	{
		return _board[coor.first][coor.second] == '.';
	}
};

