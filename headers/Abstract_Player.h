#pragma once
#include <iostream>

//
//enum DIRECTION
//{
//	RIGHT,
//	LEFT,
//	DOWN,
//	UP
//};

/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
struct Coordinate
{
	std::size_t x;
	std::size_t y;
};



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
		_position.x = x;
		_position.y = y;
	}
};