#pragma once
#include "HelperFunctions.h"

class Human_Player 
{
private:
	Coordinate m_position;
public:
    Human_Player(Coordinate position = { 1,1 }) : m_position(position) 
	{}

	bool isPlayerCaughtByEnemy(const std::vector<Coordinate>& enemyPositions) const
	{
		return std::find(enemyPositions.begin(), enemyPositions.end(), m_position) != enemyPositions.end();
	}

	Coordinate getPosition() const 
	{
		return m_position;
	}
	void setPosition(Coordinate position)
	{
		m_position = position;
	}

};