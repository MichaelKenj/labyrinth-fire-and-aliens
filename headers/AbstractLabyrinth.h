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
#include "HelperFunctions.h"

/// <summary>
/// TODO
/// do bfs from player(not from aliens
/// </summary>





class AbstractLabyrinth
{
protected:
	Human_Player m_player;
	/// Maze
	Board m_board;

	/// Size of edge of the maze
	std::size_t m_size;

	/// Coordinates of entrance in the maze
	Coordinate m_entrance;

	/// Coordinates of exits in the maze
	Coordinate m_exit1;
	Coordinate m_exit2;

public:
	//virtual void solve() const noexcept = 0;
	virtual void moveEnemies() noexcept = 0;
	virtual void generateEnemy() noexcept = 0;
	// HumanPlayer.h
	virtual bool isPlayerCaughtByEnemy() const = 0;
public:
	/// <summary>
	/// Generates square maze, depends on GAME_MODE, generates fire or aliens, and puts player on entrance cell
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="size"></param>
	explicit AbstractLabyrinth(std::size_t size = 20)
		: m_size(size)
	{
		// Filling board with '#'
		m_board.assign(m_size, std::vector<char>(m_size, '#'));

		// Generates entrance
		generateEntrance();

		// Generating maze
		generateBoard();

		m_player.setPosition(m_entrance);
		putPlayerIntoBoard();

		makeMorePaths();

		do
		{
			generateFirstExit();
			//stex mi angam miangamic won tvec, aysinqn exitn u entrance hamynkav, dra hamar avelacreci
			// es paymany
		} while (isWall(generateStartForGenerating(m_exit1)) || m_exit1 == m_entrance);
		
		m_board[m_exit1.first][m_exit1.second] = 'E';

	}

	//----------HUMAN_PLAYER-------------
	bool movePlayer(DIRECTION direction)
	{
		//                 UP, DOWN, LEFT, RIGHT
		//const int dx[4] = { -1, 1, 0, 0 };
		//const int dy[4] = { 0, 0, -1, 1 };
		Coordinate prevCoordinate = m_player.getPosition();
		Coordinate newPossibleCoordinate;
		newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[0], m_player.getPosition().second + dy[0] };

		switch (direction)
		{
		case UP:
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
				std::cout << std::setw(2) << cell;

				// Setting back white color of console
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			std::cout << std::endl;
		}
	}
	bool isValidCoord(const Coordinate& coord) const noexcept
	{
		return coord.first >= 0 && coord.first < m_size && coord.second >= 0 && coord.second < m_size;
	}

	bool isWall(Coordinate coor) const
	{
		return m_board[coor.first][coor.second] == '#';
	}

	bool isEmptySpace(Coordinate coor) const
	{
		return m_board[coor.first][coor.second] == '.';
	}

	// Game.h
	bool isWinable() const
	{
		// Should call solve() and if vector is not empty -> its winable
	}

	// Game.h
	bool isSolvableAtLeastIn5Moves() const
	{
		// Should call solve() and if vector.size() >= 5 -> its solvable in 5 moves
	}

	// Game.h
	bool isValid() const
	{
		if (isWinable() && isSolvableAtLeastIn5Moves())
			return true;
		return false;
	}

	// HumanPlayer.h
	bool isPlayerAlive() const
	{
		return m_player.isAlive();
	}

	void putPlayerIntoBoard()
	{
		m_board[m_entrance.first][m_entrance.second] = '±';
	}

	// Game.h
	bool isMazeSolved() const
	{
		return m_player.getPosition() == m_exit1 || m_player.getPosition() == m_exit2;
	}
/// <summary>
/// Functions which solves maze
/// </summary>
protected:
	/// <summary>
	/// Solving maze. If vector is empty => Maze is not solvable
	/// </summary>
	/// <returns>Vector of coordinates of winning path</returns> 
	// Game.h
	std::vector< Coordinate > solve()
	{

	}

	// Game.h
	void updatePlayerPosition()
	{
		m_board[m_player.getPosition().first][m_player.getPosition().second] = '±';
	}
	
	/// <summary>
	/// Helper functions to generate board
	/// </summary>
protected:
	// qcenq arandzin file mej
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

	// qcenq arandzin file mej
	std::size_t moveLeftOrRight(DIRECTION direction, std::size_t x) const
	{
		if (direction == RIGHT)
			return x + 1;
		else if (direction == LEFT)
			return x - 1;
		else
			return x;
	}

	// qcenq arandzin file mej
	std::size_t moveUpOrDown(int direction, int y) const
	{
		if (direction == UP)
			return y - 1;
		else if (direction == DOWN)
			return y + 1;
		else
			return y;
	}

	// qcenq arandzin file mej
	bool isGoodMove(int x, int y, DIRECTION direction) const
	{
		x = moveLeftOrRight(direction, x);
		y = moveUpOrDown(direction, y);

		if (m_board[y][x] == '.' || x >= (m_size - 1) || x <= 0 || y <= 0 || y >= (m_size - 1))
			return false;
		if (direction == UP) {
			if (m_board[y][x - 1] != '.' && m_board[y - 1][x] != '.' && m_board[y][x + 1] != '.' && m_board[y - 1][x - 1] != '.' && m_board[y - 1][x + 1] != '.')
				return true;
		}
		if (direction == DOWN) {
			if (m_board[y][x - 1] != '.' && m_board[y + 1][x] != '.' && m_board[y][x + 1] != '.' && m_board[y + 1][x - 1] != '.' && m_board[y + 1][x + 1] != '.')
				return true;
		}
		if (direction == RIGHT) {
			if (m_board[y][x + 1] != '.' && m_board[y - 1][x] != '.' && m_board[y + 1][x] != '.' && m_board[y - 1][x + 1] != '.' && m_board[y + 1][x + 1] != '.') {
				return true;
			}
		}
		if (direction == LEFT) {
			if (m_board[y][x - 1] != '.' && m_board[y - 1][x] != '.' && m_board[y + 1][x] != '.' && m_board[y - 1][x - 1] != '.' && m_board[y + 1][x - 1] != '.') {
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
				if (isGoodMove(loc_x, loc_y, static_cast<DIRECTION>(i)))
					++good_move_counter;
			}

			// if only 1 good move, move there
			if (good_move_counter == 1)
			{
				if (isGoodMove(loc_x, loc_y, UP))
					loc_y = moveUpOrDown(UP, loc_y);
				else if (isGoodMove(loc_x, loc_y, DOWN))
					loc_y = moveUpOrDown(DOWN, loc_y);
				else if (isGoodMove(loc_x, loc_y, RIGHT))
					loc_x = moveLeftOrRight(RIGHT, loc_x);
				else if (isGoodMove(loc_x, loc_y, LEFT))
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
				} while (!isGoodMove(loc_x, loc_y, static_cast<DIRECTION>(direction)));

				loc_x = moveLeftOrRight(static_cast<DIRECTION>(direction), loc_x);
				loc_y = moveUpOrDown(static_cast<DIRECTION>(direction), loc_y);
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
};