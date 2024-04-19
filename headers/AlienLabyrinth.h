#pragma once
#include "AbstractLabyrinth.h"

class AlienLabyrinth final : public AbstractLabyrinth
{
private:
	std::vector < Coordinate > m_alienPositions;
	std::vector < Coordinate > m_prevAlienPositions;
public:
	AlienLabyrinth()
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
			if (possibleEnemyPositions.size() >= 3)
			{
				std::size_t enemyCount = generateRandomNumber(3, possibleEnemyPositions.size());

				for (std::size_t i = 0; i < enemyCount; ++i)
					m_alienPositions.push_back(possibleEnemyPositions[i]);
				m_prevAlienPositions = m_alienPositions;

				for (const auto& i : m_alienPositions)
					m_board[i.first][i.second] = '&';
				m_prevBoard = m_board;
				break;
			}
		} while (!isSolvable());
	}

	bool isPlayerCaughtByEnemy() const
	{
		return m_player.isPlayerCaughtByEnemy(m_alienPositions);
	}

	void moveEnemies() noexcept
	{
		if (isPlayerCaughtByEnemy())
			return;

		for (std::size_t i = 0; i < m_alienPositions.size(); ++i)
		{
			Coordinate newPosition = findPath(m_alienPositions[i], m_player.getPosition())[1];
			m_board[m_alienPositions[i].first][m_alienPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_alienPositions[i] = newPosition;
		}
	}

	//---------------------ENEMY-GENERATION-------------------
	std::vector<Coordinate> generateEnemy()
	{
		m_alienPositions.clear();
		std::vector<Coordinate> enemyPosition;
		std::vector<Coordinate> intersections = findIntersectionCoordinate(m_winningPath);

		//Intersection - farthest
		std::vector<std::pair<Coordinate, Coordinate>> interFarthest;

		for (const auto& coordinate : intersections)
			interFarthest.push_back(std::make_pair(coordinate, findFarthestEmptyCell(coordinate)));

		for (const auto& coordinate : interFarthest)
		{
			if (enemyPosition.size() >= 5)
				break;
			if (findPath(m_entrance, coordinate.first).size() < findPath(coordinate.first, coordinate.second).size())
				enemyPosition.push_back(coordinate.second);
		}
		return enemyPosition;
	}
	void restoreEnemy() 
	{
		m_alienPositions = m_prevAlienPositions;
	}
};