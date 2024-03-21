#pragma once
#include <iostream>
#include <random>


/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;


/// <summary>
/// Generates random number from [x,y]
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>

std::size_t generateRandomNumber(std::size_t x, std::size_t y) {
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution
	std::uniform_int_distribution<> dis(x, y);


	// Generate and return the random number
	return dis(gen);
}

/// <summary>
/// Generates random coordinate from [coor1,coor2] 
/// </summary>
/// <param name="coor1"></param>
/// <param name="coor2"></param>
/// <returns></returns>
Coordinate  generateRandomCoordinate(Coordinate coor1, Coordinate coor2)
{
	Coordinate new_coor;
	new_coor.first = generateRandomNumber(coor1.first, coor2.first);
	new_coor.second = generateRandomNumber(coor1.second, coor2.second);
	return new_coor;
}

	// Generate and return the random number
	return dis(gen);
}
class Abstract_Player
{
private:
	Coordinate _position;
public:
	virtual void move() = 0;

	void setPosition(Coordinate position)
	{
		_position = position;
	}

	Coordinate getPosition() const
	{
		return _position;
	}
};