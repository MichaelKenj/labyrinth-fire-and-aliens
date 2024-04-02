#pragma once
#include "AbstractLabyrinth.h"

class FireLabyrinth final : public AbstractLabyrinth
{
private:
	/// Contains coordinates of enemy object(Fire or Aliens) in the maze
	std::vector < Coordinate > m_firePositions;
public:
	FireLabyrinth()
	{
		generateEnemy();
		m_player.setPosition(m_entrance);
		putPlayerIntoBoard();
	}

	/// <summary>
	/// Spread fire to empty neighbouring cells
	/// </summary>
	void moveEnemies() noexcept
	{
		if(isPlayerCaughtByEnemy())
		{
			return;
		}
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
				if (isValidCoord(neighbor))
				{
					if (!isWall(neighbor) && m_board[neighbor.first][neighbor.second] != '@')
					{
						newFirePositions.push_back(neighbor);
						m_board[neighbor.first][neighbor.second] = '@';
					}
				}
				
			}
		}

		// Adding new fire positions into _enemy_positions
		m_firePositions.insert(m_firePositions.end(), newFirePositions.begin(), newFirePositions.end());
	}

	/// <summary>
	/// Chooses randomly fire's count[1-3] and puts them into board
	/// </summary>
	void generateEnemy() noexcept
	{
		// Choosing fire count randomly[1-3]
		std::size_t fire_count;
		fire_count = generateRandomNumber(1, 3);

		// Generating random coordinates of fires and push_back-ing them into _enemy_positions
		for (std::size_t i = 0; i < fire_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generateRandomCoordinate(
					Coordinate{ 1,1 },
					Coordinate{ m_size - 2, m_size - 2 }
				);
			} while (isWall(new_coor));

			// Setting '@' into board
			m_board[new_coor.first][new_coor.second] = '@';

			m_firePositions.push_back(new_coor);
		}
	}

	// HumanPlayer.h 
	bool isPlayerCaughtByEnemy() const
	{
		return m_board[m_player.getPosition().first][m_player.getPosition().second] == '@';
	}
};