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

			m_board[m_exit1.first][m_exit1.second] = 'E';
			if (exitCount == 2)
				m_board[m_exit2.first][m_exit2.second] = 'E';
			m_exitCount = exitCount;

			m_winningPaths.push_back(findPath(m_entrance, m_exit1));
			if (exitCount == 2)
				m_winningPaths.push_back(findPath(m_entrance, m_exit2));

			bool isOutterLoopBraked = false;
			for (std::size_t i = 0; i < 30; ++i)
			{
				generateEnemy();
				if (isSolvableAtLeastIn5Moves())
				{
					isOutterLoopBraked = true;
					break;
				}
			}
			if (isOutterLoopBraked)
				break;

		} while (!isSolvableAtLeastIn5Moves());

		
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
			Coordinate newPosition = findPath(m_AlienPositions[i], m_player.getPosition())[1];
			m_board[m_AlienPositions[i].first][m_AlienPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_AlienPositions[i] = newPosition;
		}

	}

	
	/// <summary>
	/// Chooses randomly aliens's count(3-5) and puts them into board
	/// </summary>
	void generateEnemy() noexcept
	{
		m_AlienPositions.clear();
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

			m_AlienPositions.push_back(new_coor);
		}
	}	
};