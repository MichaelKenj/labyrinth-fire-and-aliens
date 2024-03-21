#pragma once
#include <vector>
#include "HumanPlayer.h"
#include "Game_Mode.h"
#include <iomanip>
#include <iostream>
#include<cstdlib>
#include<stack>
#include<ctime>
#include<algorithm>

using Board = std::vector<std::vector<char>>;
enum { NORTH, SOUTH, EAST, WEST };

class Labyrinth
{
private:
	Board _board;
	Human_Player _player;
	std::size_t _size;
	Coordinate _entrance;
	std::vector < Coordinate > _fire_positions;
	std::vector< Coordinate > _alien_positions;
public:
	
	Labyrinth(std::size_t _size = 20) : _size(_size)
	{
		_board.assign(_size, std::vector<char>(_size, '#'));
        generateEntrance();
		generate_board();
	}

	Labyrinth(GAME_MODE flag, std::size_t size = 20) 
		: _size(size)
	{
		_board.assign(_size, std::vector<char>(_size, '#'));
		generateEntrance();
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

	std::size_t move_east_or_west(int direction, int x) 
	{
		if (direction == EAST)
			return x + 1;
		else if (direction == WEST)
			return x - 1;
		else
			return x;
	}

	std::size_t move_north_or_south(int direction, int y) {
		if (direction == NORTH)
			return y - 1;
		else if (direction == SOUTH)
			return y + 1;
		else
			return y;
	}

	bool is_good_move(int x, int y, int direction, Board& grid) 
	{
		x = move_east_or_west(direction, x);
		y = move_north_or_south(direction, y);

		if (grid[y][x] == '.' || x >= (_size - 1) || x <= 0 || y <= 0 || y >= (_size - 1))
			return false;
		if (direction == NORTH) {
			if (grid[y][x - 1] != '.' && grid[y - 1][x] != '.' && grid[y][x + 1] != '.' && grid[y - 1][x - 1] != '.' && grid[y - 1][x + 1] != '.')
				return true;
		}
		if (direction == SOUTH) {
			if (grid[y][x - 1] != '.' && grid[y + 1][x] != '.' && grid[y][x + 1] != '.' && grid[y + 1][x - 1] != '.' && grid[y + 1][x + 1] != '.')
				return true;
		}
		if (direction == EAST) {
			if (grid[y][x + 1] != '.' && grid[y - 1][x] != '.' && grid[y + 1][x] != '.' && grid[y - 1][x + 1] != '.' && grid[y + 1][x + 1] != '.') {
				return true;
			}
		}
		if (direction == WEST) {
			if (grid[y][x - 1] != '.' && grid[y - 1][x] != '.' && grid[y + 1][x] != '.' && grid[y - 1][x - 1] != '.' && grid[y + 1][x - 1] != '.') {
				return true;
			}
		}
		return false;
	}

	void generateEntrance() 
	{
		std::size_t side = generateRandomNumber(0, 3);
		std::size_t entranceX;
		std::size_t entranceY;
		switch (side)
		{
		case 0:
			entranceX = generateRandomNumber(1, 18);
			entranceY = 0;
			break;
		case 1:
			entranceX = 19;
			entranceY = generateRandomNumber(1, 18);
			break;
		case 2:
			entranceX = generateRandomNumber(1, 18);
			entranceY = 19;
			break;
		case 3:
			entranceX = 0;
			entranceY = generateRandomNumber(1, 18);
			break;
		}
		auto entrance = std::make_pair(entranceX, entranceY);
		_entrance = entrance;

	}

	Coordinate generateStartForGenerating(Coordinate& entrance)
	{
		if (entrance.first != 0 && entrance.first != 19)
		{
			std::size_t y = (entrance.second == 0) ? entrance.second + 1 : entrance.second - 1;
			return std::make_pair(entrance.first, y);
		}
		else if (entrance.second != 0 && entrance.second != 19)
		{
			std::size_t x = (entrance.first == 0) ? entrance.first + 1 : entrance.first - 1;
			return std::make_pair(x, entrance.second);
		}
	}

	void generate_board()
	{
		srand(time(0));
		Coordinate start = generateStartForGenerating(_entrance);
		std::size_t locX = start.second;
		std::size_t locY = start.first;
		for (int i = 0; i < _size; ++i)
		{
			for (int j = 0; j < _size; ++j)
				_board[i][j] = '#';
		}

		std::stack<int> xValues;
		std::stack<int> yValues;

		std::size_t good_move_counter = 0;
		std::size_t direction = 0;

		do {
			//find n good moves
			for (int i = 0; i < 4; i++) {
				if (is_good_move(locX, locY, i, _board))
					++good_move_counter;
			}

			// if only 1 good move, move there
			if (good_move_counter == 1) 
			{
				if (is_good_move(locX, locY, NORTH, _board))
					locY = move_north_or_south(NORTH, locY);
				else if (is_good_move(locX, locY, SOUTH, _board))
					locY = move_north_or_south(SOUTH, locY);
				else if (is_good_move(locX, locY, EAST, _board))
					locX = move_east_or_west(EAST, locX);
				else if (is_good_move(locX, locY, WEST, _board))
					locX = move_east_or_west(WEST, locX);
			}

			// if no good moves, move back in stack
			else if (good_move_counter == 0) {
				locX = xValues.top();
				locY = yValues.top();
				xValues.pop();
				yValues.pop();
			}

			//if more than 1 good move, push stack
			else if (good_move_counter > 1)
			{
				xValues.push(locX);
				yValues.push(locY);
				do 
				{
					direction = rand() % 4;
				} while (!is_good_move(locX, locY, direction, _board));

				locX = move_east_or_west(direction, locX);
				locY = move_north_or_south(direction, locY);
			}
			_board[locY][locX] = '.';
			good_move_counter = 0;

		} while (!xValues.empty());
		_board[_entrance.first][_entrance.second] = '.';
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

	//---------------FIRE----------------
	void generate_fire()
	{
		// TODO
		// Implement function, which chooses randomly fire's count and put it into board
		std::size_t fire_count;
		fire_count = generateRandomNumber(1, 3);
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

};

