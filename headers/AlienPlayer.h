#pragma once
#include "AbstractPlayer.h"

class Alien_Player final : public AbstractPlayer
{
private:
	Coordinate m_position;
public:
	/// <summary>
	/// Sets playes's position
	/// </summary>
	/// <param name="position"></param>
	void setPosition(Coordinate position)
	{
		m_position = position;
	}

	/// <summary>
	/// Gives player's current position
	/// </summary>
	/// <returns></returns>
	Coordinate getPosition() const
	{
		return m_position;
	}

};