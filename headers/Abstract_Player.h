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
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(x, y);

	return dis(gen);
}

/// <summary>
/// Generates random coordinate from [coor1, coor2] 
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

class Abstract_Player
{
private:
	Coordinate m_position;
public:
	virtual ~Abstract_Player() {}

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
	virtual Coordinate get_position() const = 0;
};