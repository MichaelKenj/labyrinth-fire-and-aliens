#pragma once
#include <iostream>

/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;


class Abstract_Player
{
private:
	Coordinate _position;

public:
	virtual void move() = 0;

	Coordinate getPosition() const
	{
		return _position;
	}

	void setPosition(std::size_t x, std::size_t y)
	{
		_position.first = x;
		_position.second = y;
	}
};