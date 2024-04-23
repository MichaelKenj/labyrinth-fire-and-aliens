#pragma once
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <windows.h>
#include "HumanPlayer.h"
#include "printHelperMethods.h"
#include "Game_Mode.h"
#include "HelperFunctions.h"

/// TODO
/// Perform bfs from player(not dfs from each alien)

/*
 * Sum numbers in a vector.
 *
 * This sum is the arithmetic sum, not some other kind of sum that only
 * mathematicians have heard of.
 *
 * @param values Container whose values are summed.
 * @return sum of `values`, or 0.0 if `values` is empty.
 */


class AbstractLabyrinth
{
protected:
	Human_Player m_player;
	Board m_board;
	Board m_prevBoard;
	std::size_t m_size;
	Coordinate m_entrance;
	Coordinate m_exit;
	std::vector<Coordinate> m_winningPath;
public:
	virtual void moveEnemies() noexcept = 0;
	virtual bool isPlayerCaughtByEnemy() const = 0;
	virtual void restoreEnemy() = 0;
	virtual std::vector<Coordinate> generateEnemy() = 0;
public:
	explicit AbstractLabyrinth(std::size_t size = 21)
		: m_size(size)
		, m_exit{-1,-1}
	{}

	void printBoard() const
	{
		for (const auto& row : m_board)
		{
			for (char cell : row)
			{
				setPrintColor(cell);
				std::cout << std::setw(2) << cell;
				// Setting back white color of console
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			std::cout << std::endl;
		}
	}
	//-------------------HUMAN_PLAYER-------------------
	bool movePlayer(Direction direction)
	{
		Coordinate prevCoordinate = m_player.getPosition();
		Coordinate newPossibleCoordinate;

		switch (direction)
		{
		case Direction::eLeft:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[0],
				m_player.getPosition().second + dy[0] };
			break;
		case Direction::eUp:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[1],
				m_player.getPosition().second + dy[1] };
			break;
		case Direction::eRight:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[2],
				m_player.getPosition().second + dy[2] };
			break;
		case Direction::eDown:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[3],
				m_player.getPosition().second + dy[3] };
			break;
		}

		if (isValidCoordinate(newPossibleCoordinate))
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

	bool movePlayer(Coordinate coor)
	{
		auto prevCoordinate = m_player.getPosition();
		if (isValidCoordinate(coor))
		{
			if (!isWall(coor))
			{
				m_player.setPosition(coor);
				updatePlayerPosition();
				m_board[prevCoordinate.first][prevCoordinate.second] = '.';
				return true;
			}
		}
		return false;
	}

	void updatePlayerPosition()
	{
		m_board[m_player.getPosition().first][m_player.getPosition().second] = '±';
	}
	
	void putPlayerIntoBoard()
	{
		m_board[m_entrance.first][m_entrance.second] = '±';
	}
	//-----------------FOR-MAZE-GENERATION--------------

	//-------------FOR-ENEMY-GENERATION-----------------
	std::vector<Coordinate> findIntersectionCoordinate(const std::vector<Coordinate>& path) const
	{
		std::vector<Coordinate> resultVec;
		for (const auto& coor : path)
		{
			auto neighbouringCoors = getNeighbouringCoordinates(coor, m_board);
			for (const auto& coor2 : neighbouringCoors)
			{
				if (!isWall(coor2) && std::find(path.begin(), path.end(), coor2) == path.end())
				{
					resultVec.push_back(coor);
					break;
				}
			}
		}
		return resultVec;
	}

	Coordinate findFarthestEmptyCell(const Coordinate& start) const {
		std::vector<std::vector<bool>> visited(m_board.size(), std::vector<bool>(m_board[0].size(), false));

		// BFS queue
		std::queue<std::pair<Coordinate, std::vector<Coordinate>>> queue;
		queue.push({ start, {start} }); // Store the path as well
		visited[start.first][start.second] = true;

		// Farthest empty cell
		Coordinate farthestEmptyCell = start;

		// Perform BFS
		while (!queue.empty()) {
			auto current = queue.front();
			queue.pop();

			farthestEmptyCell = current.first;  // Update farthest empty cell

			// Explore neighboring cells
			for (const auto& neighbor : getNeighbouringCoordinates(current.first, m_board)) {
				// Check if neighbor cell is valid, not visited, not part of the winning path or current path, and not on the boundary
				if (!isValidCoordinate(neighbor) || visited[neighbor.first][neighbor.second] ||
					std::find(current.second.begin(), current.second.end(), neighbor) != current.second.end() ||
					std::find(m_winningPath.begin(), m_winningPath.end(), neighbor) != m_winningPath.end() ||
					neighbor.first == 0 || neighbor.first == m_board.size() - 1 ||
					neighbor.second == 0 || neighbor.second == m_board[0].size() - 1 ||
					isWall(neighbor))
				{
					continue;
				}

				// Visit the neighbor cell
				visited[neighbor.first][neighbor.second] = true;
				auto newPath = current.second;
				newPath.push_back(neighbor);
				queue.push({ neighbor, newPath });
			}
		}

		return farthestEmptyCell;
	}
	//--------------IDENTIFICATION-STUFF----------------
	bool isValidCoordinate(const Coordinate& coord) const noexcept
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

	bool isSolvableAtLeastIn5Moves() const
	{
		return m_winningPath.size() >= 5;
	}

	bool isMazeSolved() const
	{
		return m_player.getPosition() == m_exit;
	}


	// Is exit placed in a valid way(there is no wall in front of it, and it's not the same as an entrance)
	bool isValidExit()
	{
		return isWall(penetrateIntoLabyrinth(m_exit)) || m_exit == m_entrance;
	}
	
	void restoreBoard()
	{
		m_board = m_prevBoard;
	}

	// Getters
	Coordinate getEntrance() const
	{
		return m_entrance;
	}
	std::vector<Coordinate> getWinningPath() const
	{
		return m_winningPath;
	}

	// Setters
	void setPlayerPosition(Coordinate coor)
	{
		m_player.setPosition(coor);
	}
	void setBoard(const Board& board)
	{
		m_board = board;
	}
private:
	struct Cell
	{
		std::size_t m_row;
		std::size_t m_col;
		bool m_up;
		bool m_down;
		bool m_left;
		bool m_right;
		bool m_visited;
		Cell(std::size_t r, std::size_t c)
			: m_row(r), m_col(c), m_up(true), m_down(true), m_left(true), m_right(true), m_visited(false)
		{}
	};

/// Helper functions to generate board
protected:
	// Penetrates into labyrinth  ;)
	Coordinate penetrateIntoLabyrinth(Coordinate coordinate) const
	{
		if (coordinate.first != 0 && coordinate.first != m_size - 1)
		{
			std::size_t y = (coordinate.second == 0) ? coordinate.second + 1 : coordinate.second - 1;
			return std::make_pair(coordinate.first, y);
		}
		else if (coordinate.second != 0 && coordinate.second != m_size - 1)
		{
			std::size_t x = (coordinate.first == 0) ? coordinate.first + 1 : coordinate.first - 1;
			return std::make_pair(x, coordinate.second);
		}
	}
	void generateMaze()
	{
		const std::size_t rows = 10;
		const std::size_t cols = 10;
		std::vector<std::vector<Cell>> cells(rows, std::vector<Cell>(cols, Cell(0, 0)));

		srand(time(0));
		std::size_t startRow = rand() % rows;
		std::size_t startCol = rand() % cols;

		std::stack<std::pair<std::size_t, std::size_t>> cellStack;
		cellStack.push(std::make_pair(startRow, startCol));
		cells[startRow][startCol].m_visited = true;

		while (!cellStack.empty()) 
		{	
			std::size_t currentRow = cellStack.top().first;
			std::size_t currentCol = cellStack.top().second;
			std::vector<Direction> directions{ Direction::eUp, Direction::eDown, Direction::eLeft, Direction::eRight };
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(directions.begin(), directions.end(), std::default_random_engine(seed));
			bool hasUnvisitedNeighbor = false;

			for (Direction dir : directions) 
			{
				int newRow = currentRow;
				int newCol = currentCol;

				switch (dir) 
				{
				case Direction::eUp:
					--newRow;
					if (newRow >= 0 && !cells[newRow][newCol].m_visited)
					{
						cells[currentRow][currentCol].m_up = false;
						cells[newRow][newCol].m_down = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].m_visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case Direction::eDown:
					++newRow;
					if (newRow < rows && !cells[newRow][newCol].m_visited)
					{
						cells[currentRow][currentCol].m_down = false;
						cells[newRow][newCol].m_up = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].m_visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case Direction::eLeft:
					--newCol;
					if (newCol >= 0 && !cells[newRow][newCol].m_visited)
					{
						cells[currentRow][currentCol].m_left = false;
						cells[newRow][newCol].m_right = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].m_visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case Direction::eRight:
					++newCol;
					if (newCol < cols && !cells[newRow][newCol].m_visited)
					{
						cells[currentRow][currentCol].m_right = false;
						cells[newRow][newCol].m_left = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].m_visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				}
				if (hasUnvisitedNeighbor)
					break;
			}

			if (!hasUnvisitedNeighbor)
				cellStack.pop();
		}

		// Fill maze with walls
		m_size = 2 * rows + 1;
		m_board.assign(m_size, std::vector<char>(m_size, '#'));

		// Fill empty spaces
		for (std::size_t r = 0; r < rows; ++r)
		{
			for (std::size_t c = 0; c < cols; ++c)
			{
				std::size_t mazeRow = r * 2 + 1;
				std::size_t mazeCol = c * 2 + 1;
				m_board[mazeRow][mazeCol] = '.';

				if (!cells[r][c].m_up)
					m_board[mazeRow - 1][mazeCol] = '.';
				if (!cells[r][c].m_down)
					m_board[mazeRow + 1][mazeCol] = '.';
				if (!cells[r][c].m_left)
					m_board[mazeRow][mazeCol - 1] = '.';
				if (!cells[r][c].m_right)
					m_board[mazeRow][mazeCol + 1] = '.';
			}
		}
		m_board[m_entrance.first][m_entrance.second] = '.';
	}

	void generateEntrance()
	{
		std::size_t side = generateRandomNumber(0, 3);
		switch (side)
		{
		case 0: // left
			m_entrance.first = generateRandomNumber(1, m_size - 2);
			m_entrance.second = 0;
			break;
		case 1: // up
			m_entrance.first = m_size - 1;
			m_entrance.second = generateRandomNumber(1, m_size - 2);
			break;
		case 2: // right
			m_entrance.first = generateRandomNumber(1, m_size - 2);
			m_entrance.second = m_size - 1;
			break;
		case 3: // down
			m_entrance.first = 0;
			m_entrance.second = generateRandomNumber(1, m_size - 2);
			break;
		}
	}

	void generateExit()
	{
		std::size_t side = generateRandomNumber(0, 3);
		switch (side)
		{
		case 0: // left
			m_exit.first = generateRandomNumber(1, m_size - 2);
			m_exit.second = 0;
			break;
		case 1: // up
			m_exit.first = 0;
			m_exit.second = generateRandomNumber(1, m_size - 2);
			break;
		case 2: // right
			m_exit.first = generateRandomNumber(1, m_size - 2);
			m_exit.second = m_size - 1;
			break;
		case 3: // down
			m_exit.first = m_size - 1;
			m_exit.second = generateRandomNumber(1, m_size - 2);
			break;
		}
	}

	// Returning path vector of coordinates from start to end
	std::vector<Coordinate> findPath(Coordinate start, Coordinate end) const
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
				Coordinate next = { current.first + dx[i], current.second + dy[i] };
				if (!isValidCoordinate(next))
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
		return path;
	}

	// Solves labyrinth and restores initial data of labyrinth
	bool isSolvable()
	{
		Board copy_m_board = m_board;

		auto winPath = m_winningPath;
		int index = 1;

		while (!isPlayerCaughtByEnemy() && !isMazeSolved())
		{
			bool isPlayerMoved = false;
			isPlayerMoved = movePlayer(m_winningPath[index]);

			if (isPlayerMoved)
			{
				moveEnemies();
				++index;
			}
		}

		// Restoring
		m_board = copy_m_board;
		m_player.setPosition(m_entrance);
		restoreEnemy();

		return isPlayerCaughtByEnemy();
	}
};
