#pragma once
#include <random>

/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;
using Board = std::vector<std::vector<char>>;

//                 UP, DOWN, LEFT, RIGHT
const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, -1, 1 };

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

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