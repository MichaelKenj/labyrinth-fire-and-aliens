#pragma once
#include "AbstractLabyrinth.h"

class AlienLabyrinth final : public AbstractLabyrinth
{
private:
	std::vector < Coordinate > m_AlienPositions;
public:
	AlienLabyrinth()
	{
		do
		{
			// Filling board with '#'
			m_board.assign(m_size, std::vector<char>(m_size, '#'));

			// Generates entrance
			generateEntrance();

			generateBoard();

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
			m_winningPath = findShortestPath(m_entrance, m_exit);
			
			if (!isSolvableAtLeastIn5Moves())
				continue;

			bool isOutterLoopBraked = false;
			auto possibleEnemyPositions = newGenerateEnemy();
			if (possibleEnemyPositions.size() >= 3 && possibleEnemyPositions.size() <= 5)
			{
				m_AlienPositions = possibleEnemyPositions;
				for (auto i : m_AlienPositions)
					m_board[i.first][i.second] = '&';
				break;
			}
		} while (true);

		
	}

	// HumanPlayer.h 
	bool isPlayerCaughtByEnemy() const
	{
		return m_player.isPlayerCaughtByEnemy(m_AlienPositions);
	}

	// AlienPlayer.h
	void moveEnemies() noexcept
	{
		if (isPlayerCaughtByEnemy())
			return;

		for (std::size_t i = 0; i < m_AlienPositions.size(); ++i)
		{
			Coordinate newPosition = findShortestPath(m_AlienPositions[i], m_player.getPosition())[1];
			m_board[m_AlienPositions[i].first][m_AlienPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_AlienPositions[i] = newPosition;
		}

	}

	std::vector<Coordinate> newGenerateEnemy()
	{
		m_AlienPositions.clear();
		std::vector<Coordinate> enemyPosition;
		std::vector<Coordinate> intersections = findIntersectionCoordinate(m_winningPath);

		//Intersection - farthest
		std::map<Coordinate, Coordinate> interFarthest;
		for (const auto& coordinate : intersections)
		{
			interFarthest[coordinate] = findFarthestEmptyCell(coordinate);
		}

		for (const auto& coordinate : interFarthest)
		{
			if (findShortestPath(m_entrance, coordinate.first).size() < findShortestPath(coordinate.first, coordinate.second).size())
			{
				enemyPosition.push_back(coordinate.second);
			}
		}
		return enemyPosition;
	}
};