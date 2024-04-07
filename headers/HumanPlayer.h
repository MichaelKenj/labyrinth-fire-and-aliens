#pragma once
#include "HelperFunctions.h"

class Human_Player 
{
private:
	Coordinate m_position;
public:
    Human_Player(Coordinate position = { 1,1 }) : m_position(position) 
	{}

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
};