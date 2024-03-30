#pragma once
#include "AbstractLabyrinth.h"

class AlienLabyrinth final : public AbstractLabyrinth
{
private:

	/// Contains coordinates of enemy object(Fire or Aliens) in the maze
	std::vector < Coordinate > m_AlienPositions;
public:
	// Alien
	void moveEnemies()
	{
		if (isPlayerCaughtByAlien())
			return;

		for (std::size_t i = 0; i < m_AlienPositions.size(); ++i)
		{
			Coordinate newPosition = findPath(m_AlienPositions[i], m_player.getPosition());
			m_board[m_AlienPositions[i].first][m_AlienPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_AlienPositions[i] = newPosition;
		}
	}
};