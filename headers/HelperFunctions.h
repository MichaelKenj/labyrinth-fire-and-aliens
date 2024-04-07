#pragma once
#include <random>

/// <summary>
/// Can be (0, 0) to (19, 19)
/// </summary>
using Coordinate = std::pair<std::size_t, std::size_t>;
using Board = std::vector<std::vector<char>>;


//                 LEFT, UP, RIGHT, DOWN
const int dx[4] = { 0, -1, 0, 1 };
const int dy[4] = { -1, 0, 1, 0 };

enum DIRECTION
{
	LEFT,
	UP,
	RIGHT,
	DOWN
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


// qcenq arandzin file mej
std::vector<Coordinate> getNeighbouringCoordinates(const Coordinate coor, const Board& board)
{
	std::vector<Coordinate> res_vec;

	if (coor.first > 0)
		res_vec.push_back(Coordinate{ coor.first - 1, coor.second });

	if (coor.first < board.size() - 1)
		res_vec.push_back(Coordinate{ coor.first + 1, coor.second });

	if (coor.second > 0)
		res_vec.push_back(Coordinate{ coor.first, coor.second - 1 });

	if (coor.second < board.size() - 1)
		res_vec.push_back(Coordinate{ coor.first, coor.second + 1 });

	return res_vec;
}

void removeDuplicatesFromVector(std::vector<Coordinate>& vec) {
	std::sort(vec.begin(), vec.end()); // Sort the vector
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end()); // Remove duplicates
}