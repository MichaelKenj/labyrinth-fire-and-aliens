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
			generateEntrance();
			generateMaze();

			m_player.setPosition(m_entrance);
			putPlayerIntoBoard();

			do
			{
				generateExit();
			} while (isValidExit());

			m_board[m_exit.first][m_exit.second] = 'E';

			m_winningPath = findPath(m_entrance, m_exit);

			if (!isSolvableAtLeastIn5Moves())
				continue;

			auto possibleEnemyPositions = generateEnemy();
			if (possibleEnemyPositions.size() >= 1)
			{
				std::size_t enemyCount = generateRandomNumber(1, possibleEnemyPositions.size());

				for (std::size_t i = 0; i < enemyCount; ++i)
					m_firePositions.push_back(possibleEnemyPositions[i]);
				m_prevFirePositions = m_firePositions;

				for (const auto& i : m_firePositions)
					m_board[i.first][i.second] = '@';
				m_prevBoard = m_board;
				break;
			}
		} while (!isSolvable());
	}

	bool isPlayerCaughtByEnemy() const
	{
		return m_player.isPlayerCaughtByEnemy(m_firePositions);
	}

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
				if (isValidCoordinate(neighbor))
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

	//---------------------ENEMY-GENERATION-------------------
	std::vector<Coordinate> generateEnemy()
	{
		m_firePositions.clear();
		std::vector<Coordinate> enemyPosition;
		std::vector<Coordinate> intersections = findIntersectionCoordinate(m_winningPath);

		//Intersection - farthest
		std::vector<std::pair<Coordinate, Coordinate>> interFarthest;

		for (const auto& coordinate : intersections)
			interFarthest.push_back(std::make_pair(coordinate, findFarthestEmptyCell(coordinate)));

		for (const auto& coordinate : interFarthest)
		{
			if (enemyPosition.size() >= 3)
				break;
			if (findPath(m_entrance, coordinate.first).size() < findPath(coordinate.first, coordinate.second).size())
				enemyPosition.push_back(coordinate.second);
		}
		return enemyPosition;
	}
	void restoreEnemy()
	{
		m_firePositions = m_prevFirePositions;
	}
};