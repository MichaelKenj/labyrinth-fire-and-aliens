#pragma once
#include <vector>
#include "HumanPlayer.h"
#include "Game_Mode.h"
#include <iomanip>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <algorithm>
#include <windows.h>

using Board = std::vector<std::vector<char>>;
enum { NORTH, SOUTH, EAST, WEST };

class Labyrinth
{
private:
	/// Maze
	Board _board;
	
	/// Player in the maze 
	Human_Player _player;

	/// Size of edge of the maze
	std::size_t _size;

	/// Coordinates of entrance in the maze
	Coordinate _entrance;

	/// Contains coordinates of fire in the maze
	std::vector < Coordinate > _fire_positions;

	/// Contains coordinates of aliens in the maze
	std::vector < Coordinate > _alien_positions;
public:
	/// <summary>
	/// Also default constructor, generates square maze depends on size of edge 
	/// </summary>
	/// <param name="_size"></param>
	Labyrinth(std::size_t _size = 20) : _size(_size)
	{
		_board.assign(_size, std::vector<char>(_size, '#'));
        generate_entrance();
		generate_board();
	}

	/// <summary>
	/// Generates square maze, depends on GAME_MODE, generates fire or aliens
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="size"></param>
	Labyrinth(GAME_MODE flag, std::size_t size = 20) 
		: _size(size)
	{
		// Filling board with '#'
		_board.assign(_size, std::vector<char>(_size, '#'));

		// Generates entrance
		generate_entrance();

		// Generating maze
		generate_board();

		// Generating objects based on GAME_MODE
		if (flag == FIRE)
		{
			generate_fire();
		}
		else
		{
			generate_aliens();
		}
	}


	/// <summary>
	/// Prints board. Prints aliens as green, fire as red
	/// </summary>
	void print_board() const 
	{
		for (const auto& row : _board) 
		{
			for (char cell : row)
			{
				if (cell == '@')
				{
					// Setting color red, if cell is fire
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				}
				else if (cell == '&')
				{
					// Setting color green, if cell is alien
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				}
				std::cout <<std::setw(2) << cell;

				// Setting back white color of console
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			std::cout << std::endl;
		}
	}

	//---------------FIRE----------------
	
	/// <summary>
	/// Chooses randomly fire's count[1-3] and put it into board
	/// </summary>
	void generate_fire()
	{
		// Choosing fire count randomly[1-3]
		std::size_t fire_count;
		fire_count = generate_random_number(1, 3);

		// Generating random coordinates of fires and push_back-ing them into _fire_positions
		for (std::size_t i = 0; i < fire_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generate_random_coordinate(
					Coordinate{ 1,1 },
					Coordinate{ _board.size() - 2, _board.size() - 2 }
				);
			} while (is_wall(new_coor));

			// Setting '@' into board
			_board[new_coor.first][new_coor.second] = '@';

			_fire_positions.push_back(new_coor);
		}

	}

	/// <summary>
	/// Spread fire to empty neighbouring cells
	/// </summary>
	void spread_fire()
	{
		// Vector for neighbouring empty cells of already existing fire cells
		std::vector<Coordinate> newFirePositions;

		// Traversing on already existing fire cells
		for (const auto& firePosition : _fire_positions)
		{
			// Finding and filling vector by neighbouring cells of current cell
			auto neighbors = get_neighbouring_coordinates(firePosition);

			// Checking, if neighbour cell is a wall or its another fire, we dont push into newFirePositions
			for (const auto& neighbor : neighbors)
			{
				// Checking statement
				if (!is_wall(neighbor) && _board[neighbor.first][neighbor.second] != '@')
				{
					newFirePositions.push_back(neighbor);
					_board[neighbor.first][neighbor.second] = '@';
				}
			}
		}

		// Adding new fire positions into _fire_positions
		_fire_positions.insert(_fire_positions.end(), newFirePositions.begin(), newFirePositions.end());
	}
	//---------------FIRE----------------

	//---------------ALIEN----------------

	/// <summary>
	/// Chooses randomly aliens's count(3-5) and put it into board
	/// </summary>
	void generate_aliens()
	{
		// Choosing fire count randomly[3-5]
		std::size_t alien_count;
		alien_count = generate_random_number(3, 5);

		// Generating random coordinates of aliens and push_back-ing them into _alien_positions
		for (std::size_t i = 0; i < alien_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generate_random_coordinate(
					Coordinate{ 1,1 }, 
					Coordinate{ _board.size() - 2, _board.size() - 2 });
			} while (is_wall(new_coor));

			// Setting '&' into board
			_board[new_coor.first][new_coor.second] = '&';

			_alien_positions.push_back(new_coor);
		}
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
		return _board[_player.get_position().first][_player.get_position().second] == '@';
	}

	bool is_player_caught_by_alien() const 
	{
		return std::find(
			_alien_positions.begin(), 
			_alien_positions.end(), 
			_player.get_position()) != _alien_positions.end();
	}

	bool is_wall(Coordinate coor) const
	{
		return _board[coor.first][coor.second] == '#';
	}

	bool is_empty_space(Coordinate coor) const
	{
		return _board[coor.first][coor.second] == '.';
	}


/// <summary>
/// Helper functions to generate board
/// </summary>
private:
	Coordinate generate_start_for_generating(const Coordinate& entrance)
	{
		if (entrance.first != 0 && entrance.first != _size - 1)
		{
			std::size_t y = (entrance.second == 0) ? entrance.second + 1 : entrance.second - 1;
			return std::make_pair(entrance.first, y);
		}
		else if (entrance.second != 0 && entrance.second != _size - 1)
		{
			std::size_t x = (entrance.first == 0) ? entrance.first + 1 : entrance.first - 1;
			return std::make_pair(x, entrance.second);
		}
	}

	std::vector<Coordinate> get_neighbouring_coordinates(const Coordinate coor)
	{
		std::vector<Coordinate> res_vec;

		if (coor.first > 0)
			res_vec.push_back(Coordinate{ coor.first - 1, coor.second });

		if (coor.first < _size)
			res_vec.push_back(Coordinate{ coor.first + 1, coor.second });

		if (coor.second > 0)
			res_vec.push_back(Coordinate{ coor.first, coor.second - 1 });

		if (coor.second < _size)
			res_vec.push_back(Coordinate{ coor.first, coor.second + 1 });

		return res_vec;
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

	bool is_good_move(int x, int y, int direction, const Board& grid)
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

	void generate_entrance()
	{
		std::size_t side = generate_random_number(0, 3);
		std::size_t entrance_x;
		std::size_t entrance_y;
		switch (side)
		{
		case 0:
			entrance_x = generate_random_number(1, _size - 2);
			entrance_y = 0;
			break;
		case 1:
			entrance_x = _size - 1;
			entrance_y = generate_random_number(1, _size - 2);
			break;
		case 2:
			entrance_x = generate_random_number(1, _size - 2);
			entrance_y = _size - 1;
			break;
		case 3:
			entrance_x = 0;
			entrance_y = generate_random_number(1, _size - 2);
			break;
		}
		auto entrance = std::make_pair(entrance_x, entrance_y);
		_entrance = entrance;

	}

	void generate_board()
	{
		srand(time(0));
		Coordinate start = generate_start_for_generating(_entrance);
		std::size_t loc_x = start.second;
		std::size_t loc_y = start.first;
		for (std::size_t i = 0; i < _size; ++i)
		{
			for (std::size_t j = 0; j < _size; ++j)
				_board[i][j] = '#';
		}

		std::stack< std::size_t > x_values;
		std::stack< std::size_t > y_values;

		std::size_t good_move_counter = 0;
		std::size_t direction = 0;

		do {
			//find n good moves
			for (std::size_t i = 0; i < 4; ++i) {
				if (is_good_move(loc_x, loc_y, i, _board))
					++good_move_counter;
			}

			// if only 1 good move, move there
			if (good_move_counter == 1)
			{
				if (is_good_move(loc_x, loc_y, NORTH, _board))
					loc_y = move_north_or_south(NORTH, loc_y);
				else if (is_good_move(loc_x, loc_y, SOUTH, _board))
					loc_y = move_north_or_south(SOUTH, loc_y);
				else if (is_good_move(loc_x, loc_y, EAST, _board))
					loc_x = move_east_or_west(EAST, loc_x);
				else if (is_good_move(loc_x, loc_y, WEST, _board))
					loc_x = move_east_or_west(WEST, loc_x);
			}

			// if no good moves, move back in stack
			else if (good_move_counter == 0) {
				loc_x = x_values.top();
				loc_y = y_values.top();
				x_values.pop();
				y_values.pop();
			}

			//if more than 1 good move, push stack
			else if (good_move_counter > 1)
			{
				x_values.push(loc_x);
				y_values.push(loc_y);
				do
				{
					direction = rand() % 4;
				} while (!is_good_move(loc_x, loc_y, direction, _board));

				loc_x = move_east_or_west(direction, loc_x);
				loc_y = move_north_or_south(direction, loc_y);
			}
			_board[loc_y][loc_x] = '.';
			good_move_counter = 0;

		} while (!x_values.empty());
		_board[_entrance.first][_entrance.second] = '.';
	}
};