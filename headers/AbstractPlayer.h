#pragma once
#include "HelperFunctions.h"

class AbstractPlayer
{
private:
	Coordinate m_position;
public:
	virtual ~AbstractPlayer() {}

	virtual void move() = 0;

	/// <summary>
	/// Sets player's position
	/// </summary>
	/// <param name="position"></param>
	virtual void setPosition(Coordinate position) = 0;

	/// <summary>
	/// Gives player's coordinates
	/// </summary>
	/// <returns></returns>
	virtual Coordinate getPosition() const = 0;
};