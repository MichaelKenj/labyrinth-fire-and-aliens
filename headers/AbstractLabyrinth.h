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
#include <map>

/// <summary>
/// TODO
/// do bfs from player(not from aliens)
/// 
/// Ditarkel winning pathy, u ditarkel sax razvlletvlennianery et pathi vra
/// u et keteric gntel amenaheracvac kety u ytex dnel enemynerin
/// stugum enq wini vra, ete inch vor meky xangaruma iran iran chenq dnum ytex
/// u tenc sharunak, ete qanaky chi bavararum, petq a tazuc generacnel labirinty
/// </summary>
struct Cell {
	int row, col;
	bool up, down, left, right;
	bool visited;

	Cell(int r, int c) : row(r), col(c), up(true), down(true), left(true), right(true), visited(false) {}
};
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
	Coordinate m_exit;

	std::vector<Coordinate> m_winningPath;
public:
	//virtual void solve() const noexcept = 0;
	virtual void moveEnemies() noexcept = 0;
	//virtual void generateEnemy() noexcept = 0;
	virtual std::vector<Coordinate> newGenerateEnemy() = 0;
	// HumanPlayer.h
	virtual bool isPlayerCaughtByEnemy() const = 0;
public:
	/// <summary>
	/// Generates square maze, depends on GAME_MODE, generates fire or aliens, and puts player on entrance cell
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="size"></param>
	explicit AbstractLabyrinth(std::size_t size = 41)
		: m_size(size)
		, m_exit{-1,-1}
	{}

	//----------HUMAN_PLAYER-------------
	bool movePlayer(DIRECTION direction)
	{
		//                 LEFT, UP, RIGHT, DOWN
		//const int dx[4] = { 0, -1, 0, 1 };
		//const int dy[4] = { -1, 0, 1, 0 };
		Coordinate prevCoordinate = m_player.getPosition();
		Coordinate newPossibleCoordinate;
		newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[1], m_player.getPosition().second + dy[1] };

		switch (direction)
		{
		case LEFT:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[0], m_player.getPosition().second + dy[0] };
			break;
		case UP:
			break;
		case RIGHT:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[2], m_player.getPosition().second + dy[2] };
			break;
		case DOWN:
			newPossibleCoordinate = Coordinate{ m_player.getPosition().first + dx[3], m_player.getPosition().second + dy[3] };
			break;
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

	bool movePlayer(Coordinate coor)
	{
		auto prevCoordinate = m_player.getPosition();
		if (isValidCoord(coor))
		{
			if (!isWall(coor))
			{
				m_player.setPosition(coor);
				updatePlayerPosition();
				m_board[prevCoordinate.first][prevCoordinate.second] = '.';
				m_player.setPosition(coor);
				return true;
			}
		}
		return false;
	}
	//----------HUMAN_PLAYER-------------
	
	//----------ENEMY_GENERATION---------
	std::vector<Coordinate> findIntersectionCoordinate(const std::vector<Coordinate>& path)
	{
		std::vector<Coordinate> resultVec;
		for (auto coor : path)
		{
			auto neighbouringCoors = getNeighbouringCoordinates(coor, m_board);
			for (auto coor2 : neighbouringCoors)
			{
				if (!isWall(coor2) && std::find(path.begin(), path.end(), coor2) == path.end())
				{
					resultVec.push_back(coor);
				}
			}
		}
		return resultVec;
	}

	bool findCoordinate2D(const std::vector<std::vector<Coordinate>>& board, Coordinate value) const
	{
		for (std::size_t i = 0; i < board.size(); ++i)
		{
			for (std::size_t j = 0; j < board[i].size(); ++j)
			{
				if (board[i][j] == value)
					return true;
			}
		}
		return false;
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
			for (auto neighbor : getNeighbouringCoordinates(current.first, m_board)) {
				// Check if neighbor cell is valid, not visited, not part of the winning path or current path, and not on the boundary
				if (!isValidCoord(neighbor) || visited[neighbor.first][neighbor.second] ||
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
	//----------ENEMY_GENERATION---------

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
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
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
	bool isSolvableAtLeastIn5Moves() const
	{
		// Should call solve() and if vector.size() >= 5 -> its solvable in 5 moves
		
		return m_winningPath.size() >= 5;
	}

	// Game.h
	bool isMazeSolved() const
	{
		return m_player.getPosition() == m_exit;
	}

	// Is exit placed in valid way(there is no wall in front of it, and its not the same as entrance
	bool isValidExit(std::size_t exitcount)
	{
		if (exitcount == 1)
		{
			return isWall(generateStartForGenerating(m_exit)) || m_exit == m_entrance;
		}
		else
		{
			return isWall(generateStartForGenerating(m_exit))
				|| m_exit == m_entrance;
		}
	}
	
	void putPlayerIntoBoard()
	{
		m_board[m_entrance.first][m_entrance.second] = '±';
	}

	// Getters
	Coordinate getPlayerPosition() const
	{
		return m_player.getPosition();
	}
	Human_Player getPlayer() const {
		return m_player;
	}
	Board getBoard() const {
		return m_board;
	}
	std::size_t getSize() const {
		return m_size;
	}
	Coordinate getEntrance() const {
		return m_entrance;
	}
	Coordinate getExit1() const {
		return m_exit;
	}
	auto getWinPath() const
	{
		return m_winningPath;
	}

	// Setters
	void setPlayer(const Human_Player& player) {
		m_player = player;
	}
	void setBoard(const Board& board) {
		m_board = board;
	}
	void setSize(std::size_t size) {
		m_size = size;
	}
	void setEntrance(const Coordinate& entrance) {
		m_entrance = entrance;
	}
	void setExit1(const Coordinate& exit1) {
		m_exit = exit1;
	}

	std::vector<Coordinate> findShortestPath(const Coordinate& start, const Coordinate& end) const
	{
		std::vector<std::vector<bool>> visited(m_board.size(), std::vector<bool>(m_board[0].size(), false));
		std::vector<std::vector<int>> distance(m_board.size(), std::vector<int>(m_board[0].size(), -1)); // Initialize all distances to -1

		std::queue<Coordinate> queue;
		queue.push(start);
		visited[start.first][start.second] = true;
		distance[start.first][start.second] = 0; // Distance from start to start is 0

		while (!queue.empty()) {
			Coordinate current = queue.front();
			queue.pop();

			if (current == end) {
				break;
			}

			for (std::size_t i = 0; i < 4; ++i)
			{
				Coordinate next = { current.first + dx[i], current.second + dy[i] };
				if (!isValidCoord(next))
				{
					continue; // Out of bounds
				}
				if (m_board[next.first][next.second] == '#' || visited[next.first][next.second]) {
					continue; // Wall or already visited
				}
				visited[next.first][next.second] = true;
				distance[next.first][next.second] = distance[current.first][current.second] + 1; // Increment distance
				queue.push(next);
			}
		}

		// Reconstruct the shortest path
		std::vector<Coordinate> path;
		Coordinate current = end;
		int shortestDistance = distance[end.first][end.second];
		while (shortestDistance > 0) {
			path.push_back(current);
			for (std::size_t i = 0; i < 4; ++i)
			{
				Coordinate next = { current.first + dx[i], current.second + dy[i] };
				if (isValidCoord(next) && distance[next.first][next.second] == shortestDistance - 1) {
					current = next;
					shortestDistance--;
					break;
				}
			}
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());
		return path;
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
	// Mi hatik ners a mtnum
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
	std::size_t moveUpOrDown(std::size_t direction, std::size_t y) const
	{
		if (direction == UP)
			return y - 1;
		else if (direction == DOWN)
			return y + 1;
		else
			return y;
	}

	// qcenq arandzin file mej
	bool isGoodMove(int x,int y, DIRECTION direction) const
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

	std::size_t generateExits()
	{
		std::size_t exit_count = 1;
		DIRECTION side1 = static_cast<DIRECTION>(generateRandomNumber(0, 3));
		Coordinate exit1;
		switch (side1)
		{
		case UP:
			exit1.first = 0;
			exit1.second = generateRandomNumber(1, m_size - 2);
			break;
		case DOWN:
			exit1.first = m_size - 1;
			exit1.second = generateRandomNumber(1, m_size - 2);
			break;
		case LEFT:
			exit1.first = generateRandomNumber(1, m_size - 2);
			exit1.second = 0;
			break;
		case RIGHT:
			exit1.first = generateRandomNumber(1, m_size - 2);
			exit1.second = m_size - 1;
			break;
		}
		m_exit = exit1;
		return exit_count;
	}
	const int rows = 20;
	const int cols = 20;
	const char wall = '#';
	const char space = '.';
	void generateMaze() 
	{
		std::vector<std::vector<Cell>> cells(rows, std::vector<Cell>(cols, Cell(0, 0)));

		srand(time(0));
		int startRow = rand() % rows;
		int startCol = rand() % cols;

		std::stack<std::pair<int, int>> cellStack;
		cellStack.push(std::make_pair(startRow, startCol));
		cells[startRow][startCol].visited = true;

		while (!cellStack.empty()) {
			int currentRow = cellStack.top().first;
			int currentCol = cellStack.top().second;
			std::vector<DIRECTION> directions{ UP, DOWN, LEFT, RIGHT };
			random_shuffle(directions.begin(), directions.end());
			bool hasUnvisitedNeighbor = false;

			for (DIRECTION dir : directions) {
				int newRow = currentRow;
				int newCol = currentCol;

				switch (dir) {
				case UP:
					newRow--;
					if (newRow >= 0 && !cells[newRow][newCol].visited) 
					{
						cells[currentRow][currentCol].up = false;
						cells[newRow][newCol].down = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case DOWN:
					newRow++;
					if (newRow < rows && !cells[newRow][newCol].visited) 
					{
						cells[currentRow][currentCol].down = false;
						cells[newRow][newCol].up = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case LEFT:
					newCol--;
					if (newCol >= 0 && !cells[newRow][newCol].visited)
					{
						cells[currentRow][currentCol].left = false;
						cells[newRow][newCol].right = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].visited = true;
						hasUnvisitedNeighbor = true;
					}
					break;
				case RIGHT:
					newCol++;
					if (newCol < cols && !cells[newRow][newCol].visited) 
					{
						cells[currentRow][currentCol].right = false;
						cells[newRow][newCol].left = false;
						cellStack.push(std::make_pair(newRow, newCol));
						cells[newRow][newCol].visited = true;
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
		m_board.assign(rows * 2 + 1, std::vector<char>(cols * 2 + 1, wall));

		// Fill empty spaces
		for (int r = 0; r < rows; ++r)
		{
			for (int c = 0; c < cols; ++c) 
			{
				int mazeRow = r * 2 + 1;
				int mazeCol = c * 2 + 1;
				m_board[mazeRow][mazeCol] = space;

				if (!cells[r][c].up)
					m_board[mazeRow - 1][mazeCol] = space;
				if (!cells[r][c].down)
					m_board[mazeRow + 1][mazeCol] = space;
				if (!cells[r][c].left)
					m_board[mazeRow][mazeCol - 1] = space;
				if (!cells[r][c].right)
					m_board[mazeRow][mazeCol + 1] = space;
			}
		}
		m_board[m_entrance.first][m_entrance.second] = '.';
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
		return path;
	}
};
