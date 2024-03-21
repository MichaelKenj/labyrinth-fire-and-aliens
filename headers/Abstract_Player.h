#pragma once
#include <iostream>

/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;


class Abstract_Player
{
public:
	virtual void move() = 0;
};