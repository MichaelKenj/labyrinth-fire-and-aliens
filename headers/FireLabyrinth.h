#pragma once
#include "AbstractLabyrinth.h"

class FireLabyrinth final : public AbstractLabyrinth
{
private:
	/// Contains coordinates of enemy object(Fire or Aliens) in the maze
	std::vector < Coordinate > m_firePositions;
public:
	/// <summary>
	/// Spread fire to empty neighbouring cells
	/// </summary>
	void moveEnemies()
	{
		// Vector for neighbouring empty cells of already existing fire cells
		std::vector<Coordinate> newFirePositions;

		// Traversing on already existing fire cells
		for (const auto& firePosition : m_firePositions)
		{
			// Finding and filling vector by neighbouring cells of current cell
			auto neighbors = getNeighbouringCoordinates(firePosition);

			// Checking, if neighbour cell is a wall or its another fire, we dont push into newFirePositions
			for (const auto& neighbor : neighbors)
			{
				// Checking statement
				if (!isWall(neighbor) && m_board[neighbor.first][neighbor.second] != '@')
				{
					newFirePositions.push_back(neighbor);
					m_board[neighbor.first][neighbor.second] = '@';
				}
			}
		}

		// Adding new fire positions into _enemy_positions
		m_firePositions.insert(m_firePositions.end(), newFirePositions.begin(), newFirePositions.end());
	}


};