#pragma once
#include "AbstractPlayer.h"
#include "HelperFunctions.h"

class Human_Player final : public AbstractPlayer
{
private:
	Coordinate m_position;
	bool m_isAlive;
public:
    Human_Player(Coordinate position = { 1,1 }) : m_isAlive(true), m_position(position) {}

	/// <summary>
	/// Changes player's position depends on direction
	/// </summary>
	void move() 
	{
		// TODO
		// Implement function, which changes position in board
		// tox veradaracni possibleNewCoordinate, heto mi tex stugenq inqy valid a te che, ete ha
		// uremn kanenq setPosition()
	}

	void setPosition(Coordinate position)
	{
		m_position = position;
	}

	Coordinate getPosition() const 
	{
		return m_position;
	}

	bool isPlayerCaughtByEnemy(const std::vector<Coordinate>& enemyPositions) const
	{
		return std::find(enemyPositions.begin(), enemyPositions.end(), m_position) != enemyPositions.end();
		
	}

	bool isPlayerCaughtByEnemy(const std::vector<Alien_Player>& enemyPositions) const
	{
		for (std::size_t i = 0; i < enemyPositions.size(); ++i)
		{
			if (enemyPositions[i].getPosition() == m_position)
				return true;
		}
		return false;
	}

	bool isAlive() const
	{
		return m_isAlive;
	}

	// Is this useless?
	void killPlayer()
	{
		m_isAlive = false;
	}
};