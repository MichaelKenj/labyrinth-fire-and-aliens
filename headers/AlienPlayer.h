#pragma once
#include "AbstractPlayer.h"

class Alien_Player final : public AbstractPlayer
{
private:
	Coordinate m_position;
public:
	void setPosition(Coordinate position)
	{
		m_position = position;
	}

	Coordinate getPosition() const
	{
		return m_position;
	}
};