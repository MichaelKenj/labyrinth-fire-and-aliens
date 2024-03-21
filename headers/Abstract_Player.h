#pragma once
#include <iostream>
#include <random>
/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;

std::size_t generateRandomNumber(std::size_t x, std::size_t y) {
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution
	std::uniform_int_distribution<> dis(x, y);

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