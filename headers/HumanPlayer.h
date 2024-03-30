#pragma once
#include "AbstractPlayer.h"

class Human_Player final : public AbstractPlayer
{
private:
	Coordinate m_position;
	bool m_isAlive;
public:
    Human_Player(Coordinate position = { 1,1 }) : m_isAlive(true), m_position(position) {}

	/// <summary>
	/// Changes player's position depends on pressed button
	/// </summary>
	void move() 
	{
		// TODO
		// Implement function, which changes position in board
	}

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