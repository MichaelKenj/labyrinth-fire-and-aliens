#pragma once
#include "AbstractPlayer.h"

class Alien_Player final : public AbstractPlayer
{
private:
	Coordinate m_position;
public:
	Alien_Player(Coordinate coord) : m_position(coord)
	{}
	void setPosition(Coordinate position)
	{
		m_position = position;
	}

	void move()
	{

	}

	Coordinate getPosition() const
	{
		return m_position;
	}
};