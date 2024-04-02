#pragma once
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <queue>
#include <algorithm>
#include <windows.h>
#include "HumanPlayer.h"
#include "printHelperMethods.h"
#include "Game_Mode.h"

/// <summary>
/// TODO
/// Separate Labirynth class into 2 class depends on game_mode
/// do bfs from player(not from aliens
/// </summary>

using Board = std::vector<std::vector<char>>;

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//                 UP, DOWN, LEFT, RIGHT
const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, -1, 1 };

class Labyrinth
{
private:
	/// Maze
	Board m_board;
	
	/// Player in the maze 
	Human_Player m_player; // ±

	/// Size of edge of the maze
	std::size_t m_size;

	/// Coordinates of entrance in the maze
	Coordinate m_entrance;
	
	/// Coordinates of exits in the maze
	Coordinate m_exit1;
	Coordinate m_exit2;

	/// Contains coordinates of enemy object(Fire or Aliens) in the maze
	std::vector < Coordinate > m_enemyPositions;
public:
	/// <summary>
	/// Generates square maze, depends on GAME_MODE, generates fire or aliens, and puts player on entrance cell
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="size"></param>
	Labyrinth(GAME_MODE flag = FIRE, std::size_t size = 20) 
		: m_size(size)
	{
		// Filling board with '#'
		m_board.assign(m_size, std::vector<char>(m_size, '#'));

		// Generates entrance
		generateEntrance();

		// Generating maze
		generateBoard();
		makeMorePaths();
		do
		{
			generateFirstExit();
		} while (isWall(generateStartForGenerating(m_exit1)));
		m_board[m_exit1.first][m_exit1.second] = 'E';
		// Generating objects based on GAME_MODE
		if (flag == FIRE)
		{
			generateFire();
		}
		else
		{
			generateAliens();
		}
		m_player.setPosition(m_entrance);
		putPlayerIntoBoard();
	}

	/// <summary>
	/// Prints board. Prints aliens as green, fire as red, player as blue
	/// </summary>
	void printBoard() const
	{
		for (const auto& row : m_board) 
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
				else if (cell == '±')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
				}
				std::cout <<std::setw(2) << cell;

				// Setting back white color of console
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			std::cout << std::endl;
		}
	}
	//----------HUMAN_PLAYER-------------

	/// <summary>
	/// Changes player's position depends on direction
	/// </summary>
	/// <param name="direction"></param>
	bool movePlayer(DIRECTION direction)
	{
		//                 UP, DOWN, LEFT, RIGHT
		//const int dx[4] = { -1, 1, 0, 0 };
		//const int dy[4] = { 0, 0, -1, 1 };
		Coordinate prevCoordinate = m_player.getPosition();
		Coordinate newPossibleCoordinate;

		switch (direction)
		{
		case UP:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[0], m_player.getPosition().second + dy[0] };
			break;
			// -1 0
		case DOWN:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[1], m_player.getPosition().second + dy[1] };
			break;
			// +1 0
		case LEFT:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[2], m_player.getPosition().second + dy[2] };
			break;
			// 0 -1
		case RIGHT:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[3], m_player.getPosition().second + dy[3] };
			break;
			// 0 +1
		}
		if (isValidCoord(newPossibleCoordinate))
		{
			if (!isWall(newPossibleCoordinate))
			{
				m_player.setPosition(newPossibleCoordinate);
				updatePlayerPosition();
				m_board[prevCoordinate.first][prevCoordinate.second] = '.';
				return true;
			}
		}
		return false;
	}
	//----------HUMAN_PLAYER-------------

	//---------------ENEMY----------------

	/// <summary>
	/// Doing move logic based on game mode(Spreading fire if mode is FIRE, moving aliens on the other hand)
	/// </summary>
	/// <param name="game_mode"></param>
	void moveEnemies(GAME_MODE game_mode)
	{
		game_mode == FIRE ? spreadFire() : moveAliens();
	}
	//---------------ENEMY----------------

	bool isWinable() const
	{
		// Should call solve() and if vector is not empty -> its winable
	}

	bool isSolvableAtLeastIn5Moves() const
	{
		// Should call solve() and if vector.size() >= 5 -> its solvable in 5 moves
	}

	bool isValid() const
	{
		if(isWinable() && isSolvableAtLeastIn5Moves())
			return true;
		return false;
	}

	bool isPlayerOnFire() const 
	{
		return m_board[m_player.getPosition().first][m_player.getPosition().second] == '@';
	}

	bool isPlayerCaughtByAlien() const 
	{
		return std::find(
			m_enemyPositions.begin(), 
			m_enemyPositions.end(), 
			m_player.getPosition()) != m_enemyPositions.end();
	}

	bool isWall(Coordinate coor) const
	{
		return m_board[coor.first][coor.second] == '#';
	}
	bool isValidCoord(Coordinate coor) const
	{
		return coor.first >= 0 && coor.first < m_size && coor.second >= 0 && coor.second < m_size;
	}
	bool isEmptySpace(Coordinate coor) const
	{
		return m_board[coor.first][coor.second] == '.';
	}

	bool isPlayerAlive() const
	{
		return !isPlayerCaughtByAlien() && !isPlayerOnFire();
	}

	bool isMazeSolved() const
	{
		return m_player.getPosition() == m_exit1 || m_player.getPosition() == m_exit2;
	}
/// <summary>
/// Functions which solves maze
/// </summary>
private:
	/// <summary>
	/// Solving maze. If vector is empty => Maze is not solvable
	/// </summary>
	/// <returns>Vector of coordinates of winning path</returns>
	std::vector< Coordinate > solve()
	{

	}

/// <summary>
/// Helper functions to generate board
/// </summary>
private:
	Coordinate generateStartForGenerating(const Coordinate entrance) const
	{
		if (entrance.first != 0 && entrance.first != m_size - 1)
		{
			std::size_t y = (entrance.second == 0) ? entrance.second + 1 : entrance.second - 1;
			return std::make_pair(entrance.first, y);
		}
		else if (entrance.second != 0 && entrance.second != m_size - 1)
		{
			std::size_t x = (entrance.first == 0) ? entrance.first + 1 : entrance.first - 1;
			return std::make_pair(x, entrance.second);
		}
	}

	std::vector<Coordinate> getNeighbouringCoordinates(const Coordinate coor)
	{
		std::vector<Coordinate> res_vec;

		if (coor.first > 0)
			res_vec.push_back(Coordinate{ coor.first - 1, coor.second });

		if (coor.first < m_size)
			res_vec.push_back(Coordinate{ coor.first + 1, coor.second });

		if (coor.second > 0)
			res_vec.push_back(Coordinate{ coor.first, coor.second - 1 });

		if (coor.second < m_size)
			res_vec.push_back(Coordinate{ coor.first, coor.second + 1 });

		return res_vec;
	}

	std::size_t moveLeftOrRight(int direction, int x) const
	{
		if (direction == RIGHT)
			return x + 1;
		else if (direction == LEFT)
			return x - 1;
		else
			return x;
	}

	std::size_t moveUpOrDown(int direction, int y) const
	{
		if (direction == UP)
			return y - 1;
		else if (direction == DOWN)
			return y + 1;
		else
			return y;
	}

	bool isGoodMove(int x, int y, int direction, const Board& grid) const
	{
		x = moveLeftOrRight(direction, x);
		y = moveUpOrDown(direction, y);

		if (grid[y][x] == '.' || x >= (m_size - 1) || x <= 0 || y <= 0 || y >= (m_size - 1))
			return false;
		if (direction == UP) {
			if (grid[y][x - 1] != '.' && grid[y - 1][x] != '.' && grid[y][x + 1] != '.' && grid[y - 1][x - 1] != '.' && grid[y - 1][x + 1] != '.')
				return true;
		}
		if (direction == DOWN) {
			if (grid[y][x - 1] != '.' && grid[y + 1][x] != '.' && grid[y][x + 1] != '.' && grid[y + 1][x - 1] != '.' && grid[y + 1][x + 1] != '.')
				return true;
		}
		if (direction == RIGHT) {
			if (grid[y][x + 1] != '.' && grid[y - 1][x] != '.' && grid[y + 1][x] != '.' && grid[y - 1][x + 1] != '.' && grid[y + 1][x + 1] != '.') {
				return true;
			}
		}
		if (direction == LEFT) {
			if (grid[y][x - 1] != '.' && grid[y - 1][x] != '.' && grid[y + 1][x] != '.' && grid[y - 1][x - 1] != '.' && grid[y + 1][x - 1] != '.') {
				return true;
			}
		}
		return false;
	}

	// change to enum
	void generateEntrance()
	{
		std::size_t side = generateRandomNumber(0, 3);
		std::size_t entrance_x;
		std::size_t entrance_y;
		switch (side)
		{
		case 0:
			entrance_x = generateRandomNumber(1, m_size - 2);
			entrance_y = 0;
			break;
		case 1:
			entrance_x = m_size - 1;
			entrance_y = generateRandomNumber(1, m_size - 2);
			break;
		case 2:
			entrance_x = generateRandomNumber(1, m_size - 2);
			entrance_y = m_size - 1;
			break;
		case 3:
			entrance_x = 0;
			entrance_y = generateRandomNumber(1, m_size - 2);
			break;
		}
		auto entrance = std::make_pair(entrance_x, entrance_y);
		m_entrance = entrance;

	}

	void generateSecondExit()
	{

	}

	void generateFirstExit()
	{
		// TODO
		// change exit char
		//std::size_t exit_count = generateRandomNumber(1, 2);
		DIRECTION side = static_cast<DIRECTION>(generateRandomNumber(0, 3));
		std::size_t exit_x;
		std::size_t exit_y;
		switch (side)
		{
		case UP:
			exit_x = 0;
			exit_y = generateRandomNumber(1, m_size - 2);
			break;
		case DOWN:
			exit_x = m_size - 1;
			exit_y = generateRandomNumber(1, m_size - 2);
			break;
		case LEFT:
			exit_x = generateRandomNumber(1, m_size - 2);
			exit_y = 0;
			
			break;
		case RIGHT:
			
			exit_x = generateRandomNumber(1, m_size - 2);
			exit_y = m_size - 1;
			break;
		}
		auto exit = std::make_pair(exit_x, exit_y);
		m_exit1 = exit;
	}

	void generateBoard()
	{
		srand(time(0));
		Coordinate start = generateStartForGenerating(m_entrance);
		std::size_t loc_x = start.second;
		std::size_t loc_y = start.first;
		for (std::size_t i = 0; i < m_size; ++i)
		{
			for (std::size_t j = 0; j < m_size; ++j)
				m_board[i][j] = '#';
		}

		std::stack< std::size_t > x_values;
		std::stack< std::size_t > y_values;

		std::size_t good_move_counter = 0;
		std::size_t direction = 0;

		do 
		{
			//find n good moves
			for (std::size_t i = 0; i < 4; ++i) 
			{
				if (isGoodMove(loc_x, loc_y, i, m_board))
					++good_move_counter;
			}

			// if only 1 good move, move there
			if (good_move_counter == 1)
			{
				if (isGoodMove(loc_x, loc_y, UP, m_board))
					loc_y = moveUpOrDown(UP, loc_y);
				else if (isGoodMove(loc_x, loc_y, DOWN, m_board))
					loc_y = moveUpOrDown(DOWN, loc_y);
				else if (isGoodMove(loc_x, loc_y, RIGHT, m_board))
					loc_x = moveLeftOrRight(RIGHT, loc_x);
				else if (isGoodMove(loc_x, loc_y, LEFT, m_board))
					loc_x = moveLeftOrRight(LEFT, loc_x);
			}

			// if no good moves, move back in stack
			else if (good_move_counter == 0) 
			{
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
				} while (!isGoodMove(loc_x, loc_y, direction, m_board));

				loc_x = moveLeftOrRight(direction, loc_x);
				loc_y = moveUpOrDown(direction, loc_y);
			}
			m_board[loc_y][loc_x] = '.';
			good_move_counter = 0;

		} while (!x_values.empty());
		m_board[m_entrance.first][m_entrance.second] = '.';
	}
	void makeMorePaths()
	{
		Coordinate current;
		for (int i = 0; i < 100; ++i)
		{
			// Calculate new coordinates based on the direction
			current = generateRandomCoordinate({ 1,1 }, { m_size - 2, m_size - 2 });
			m_board[current.first][current.second] = '.';
		}
	}

/// <summary>
/// Helper functions for object movements
/// </summary>
private:
	Coordinate findPath(Coordinate start, Coordinate end)
	{
		std::vector<std::vector<bool>> visited(m_board.size(), std::vector<bool>(m_board[0].size(), false));
		std::vector<std::vector<Coordinate>> parent(m_board.size(), std::vector<Coordinate>(m_board[0].size()));

		std::queue<Coordinate> queue;
		queue.push(start);
		visited[start.first][start.second] = true;
		parent[start.first][start.second] = start;

		while (!queue.empty()) {
			Coordinate current = queue.front();
			queue.pop();

			if (current == end) {
				break;
			}

			for (std::size_t i = 0; i < 4; ++i)
			{
				Coordinate next = { current.first + dx[i], current.second + dy[i]};
				if (!isValidCoord(next)) 
				{
					continue; // Out of bounds
				}
				if (m_board[next.first][next.second] == '#' || visited[next.first][next.second]) {
					continue; // Wall or already visited
				}
				visited[next.first][next.second] = true;
				parent[next.first][next.second] = current;
				queue.push(next);
			}
		}

		// Reconstruct the path
		std::vector<Coordinate> path;
		Coordinate current = end;
		while (current != start) {
			path.push_back(current);
			current = parent[current.first][current.second];
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());
		return path[1];
	}

	//-------------------------FIRE---------------------------

/// <summary>
/// Chooses randomly fire's count[1-3] and puts them into board
/// </summary>
	void generateFire()
	{
		// Choosing fire count randomly[1-3]
		std::size_t fire_count;
		fire_count = generateRandomNumber(1, 3);

		// Generating random coordinates of fires and push_back-ing them into _enemy_positions
		for (std::size_t i = 0; i < fire_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generateRandomCoordinate(
					Coordinate{ 1,1 },
					Coordinate{ m_size - 2, m_size - 2 }
				);
			} while (isWall(new_coor));

			// Setting '@' into board
			m_board[new_coor.first][new_coor.second] = '@';

			m_enemyPositions.push_back(new_coor);
		}
	}

	/// <summary>
	/// Spread fire to empty neighbouring cells
	/// </summary>
	void spreadFire()
	{
		// Vector for neighbouring empty cells of already existing fire cells
		std::vector<Coordinate> newFirePositions;

		// Traversing on already existing fire cells
		for (const auto& firePosition : m_enemyPositions)
		{
			// Finding and filling vector by neighbouring cells of current cell
			auto neighbors = getNeighbouringCoordinates(firePosition);

			// Checking, if neighbour cell is a wall or its another fire, we dont push into newFirePositions
			for (const auto& neighbor : neighbors)
			{
				// Checking statement
				if (isValidCoord(neighbor))
				{
					if (!isWall(neighbor) && m_board[neighbor.first][neighbor.second] != '@')
					{
						newFirePositions.push_back(neighbor);
						m_board[neighbor.first][neighbor.second] = '@';
					}
				}
				
			}
		}

		// Adding new fire positions into _enemy_positions
		m_enemyPositions.insert(m_enemyPositions.end(), newFirePositions.begin(), newFirePositions.end());
	}
	//-------------------------FIRE---------------------------

	//-------------------------ALIEN--------------------------

	/// <summary>
	/// Chooses randomly aliens's count(3-5) and puts them into board
	/// </summary>
	void generateAliens()
	{
		// Choosing fire count randomly[3-5]
		std::size_t alien_count;
		alien_count = generateRandomNumber(3, 5);

		// Generating random coordinates of aliens and push_back-ing them into _enemy_positions
		for (std::size_t i = 0; i < alien_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generateRandomCoordinate(
					Coordinate{ 1,1 },
					Coordinate{ m_size - 2, m_size - 2 });
			} while (isWall(new_coor));

			// Setting '&' into board
			m_board[new_coor.first][new_coor.second] = '&';

			m_enemyPositions.push_back(new_coor);
		}
	}

	void moveAliens()
	{
		if (isPlayerCaughtByAlien())
			return;

		for (std::size_t i = 0; i < m_enemyPositions.size(); ++i)
		{
			Coordinate newPosition = findPath(m_enemyPositions[i], m_player.getPosition());
			m_board[m_enemyPositions[i].first][m_enemyPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_enemyPositions[i] = newPosition;
		}
	}
	//-------------------------ALIEN--------------------------

	//-------------------------PLAYER--------------------------

	/// <summary>
	/// Puts player on the start-entrance cell
	/// </summary>
	void putPlayerIntoBoard()
	{
		m_board[m_entrance.first][m_entrance.second] = '±';
	}

	/// <summary>
	/// Updates player's position on the board
	/// </summary>
	void updatePlayerPosition()
	{
		m_board[m_player.getPosition().first][m_player.getPosition().second] = '±';
	}
	//-------------------------PLAYER--------------------------
};