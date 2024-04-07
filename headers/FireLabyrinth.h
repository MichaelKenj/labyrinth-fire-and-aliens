#pragma once
#include "AbstractLabyrinth.h"

class FireLabyrinth final : public AbstractLabyrinth
{
private:
	std::vector < Coordinate > m_firePositions;
	std::vector < Coordinate > m_prevFirePositions;

public:
	FireLabyrinth()
	{
		do
		{
			// Filling board with '#'
			m_board.assign(m_size, std::vector<char>(m_size, '#'));

			// Generates entrance
			generateEntrance();

			generateMaze();

			m_player.setPosition(m_entrance);
			putPlayerIntoBoard();

			std::size_t exitCount;
			do
			{
				exitCount = generateExits();
			} while (isValidExit(exitCount));

			m_board[m_exit.first][m_exit.second] = 'E';

			// If exits too near to entrance, generate another board

			m_winningPath.clear();
			m_winningPath = findPath(m_entrance, m_exit);

			if (!isSolvableAtLeastIn5Moves())
				continue;

			auto possibleEnemyPositions = generateEnemy();
			//removeDuplicatesFromVector(possibleEnemyPositions);
			if (possibleEnemyPositions.size() >= 3 && possibleEnemyPositions.size() <= 5)
			{
				m_firePositions = possibleEnemyPositions;
				m_prevFirePositions = m_firePositions;
				for (auto i : m_firePositions)
					m_board[i.first][i.second] = '@';
				m_prevBoard = m_board;
				break;
			}

		} while (!isSolvable());

	}

	/// <summary>
	/// Spread fire to empty neighbouring cells
	/// </summary>
	void moveEnemies() noexcept
	{
		if (isPlayerCaughtByEnemy())
		{
			return;
		}
		// Vector for neighbouring empty cells of already existing fire cells
		std::vector<Coordinate> newFirePositions;

		// Traversing on already existing fire cells
		for (const auto& firePosition : m_firePositions)
		{
			// Finding and filling vector by neighbouring cells of current cell
			auto neighbors = getNeighbouringCoordinates(firePosition, m_board);

			// Checking, if neighbour cell is a wall or its another fire, we dont push into newFirePositions
			for (const auto& neighbor : neighbors)
			{
				// Checking statement
				if (isValidCoord(neighbor))
				{
					if (!isWall(neighbor) && m_board[neighbor.first][neighbor.second] != '@'
						&& neighbor != m_exit)
					{
						newFirePositions.push_back(neighbor);
						m_board[neighbor.first][neighbor.second] = '@';
					}
				}

			}
		}

		// Adding new fire positions into m_firePositions
		m_firePositions.insert(m_firePositions.end(), newFirePositions.begin(), newFirePositions.end());
	}

	std::vector<Coordinate> getEnemy() const
	{
		return m_firePositions;
	}

	std::vector<Coordinate> generateEnemy()
	{
		m_firePositions.clear();
		std::vector<Coordinate> enemyPosition;
		std::vector<Coordinate> intersections = findIntersectionCoordinate(m_winningPath);
		std::vector<Coordinate> farthestCooridantes;

		//Intersection - farthest
		std::map<Coordinate, Coordinate> interFarthest;
		for (const auto& coordinate : intersections)
		{
			interFarthest[coordinate] = findFarthestEmptyCell(coordinate);
		}

		for (const auto& coordinate : interFarthest)
		{
			if (findPath(m_player.getPosition(), coordinate.first).size() > findPath(coordinate.first, coordinate.second).size())
			{
				enemyPosition.push_back(coordinate.second);
			}
		}
		return enemyPosition;
	}

	void restoreEnemy()
	{
		m_firePositions = m_prevFirePositions;
	}
	// HumanPlayer.h 
	bool isPlayerCaughtByEnemy() const
	{
		return m_player.isPlayerCaughtByEnemy(m_firePositions);
	}
};