#pragma once
#include "AbstractLabyrinth.h"

class AlienLabyrinth final : public AbstractLabyrinth
{
private:
	std::vector < Coordinate > m_AlienPositions;
public:
	AlienLabyrinth()
	{
		generateEnemy();
	}

	// HumanPlayer.h 
	bool isPlayerCaughtByEnemy() const
	{
		return std::find(
			m_AlienPositions.begin(),
			m_AlienPositions.end(),
			m_player.getPosition()) != m_AlienPositions.end();
	}
	// AlienPlayer.h
	void moveEnemies() noexcept
	{
		if (isPlayerCaughtByEnemy())
			return;

		for (std::size_t i = 0; i < m_AlienPositions.size(); ++i)
		{
			Coordinate newPosition = findPath(m_AlienPositions[i], m_player.getPosition());
			m_board[m_AlienPositions[i].first][m_AlienPositions[i].second] = '.';
			m_board[newPosition.first][newPosition.second] = '&';
			m_AlienPositions[i] = newPosition;
		}
	}

	/// <summary>
	/// Chooses randomly aliens's count(3-5) and puts them into board
	/// </summary>
	void generateEnemy() noexcept
	{
		// Choosing fire count randomly[3-5]
		std::size_t alien_count;
		alien_count = generateRandomNumber(3, 5);

		// Generating random coordinates of aliens and push_back-ing them into _enemy_positions
		for (std::size_t i = 0; i < alien_count; ++i)
		{
			Coordinate new_coor;

			// Generating new coordinates, while coordinate is a wall
			do
			{
				new_coor = generateRandomCoordinate(
					Coordinate{ 1,1 },
					Coordinate{ m_size - 2, m_size - 2 });
			} while (isWall(new_coor));

			// Setting '&' into board
			m_board[new_coor.first][new_coor.second] = '&';

			m_AlienPositions.push_back(new_coor);
		}
	}
/// <summary>
/// Helper functions for object movements
/// </summary>
private:
	Coordinate findPath(Coordinate start, Coordinate end)
	{
		std::vector<std::vector<bool>> visited(m_board.size(), std::vector<bool>(m_board[0].size(), false));
		std::vector<std::vector<Coordinate>> parent(m_board.size(), std::vector<Coordinate>(m_board[0].size()));

		std::queue<Coordinate> queue;
		queue.push(start);
		visited[start.first][start.second] = true;
		parent[start.first][start.second] = start;

		while (!queue.empty()) {
			Coordinate current = queue.front();
			queue.pop();

			if (current == end) {
				break;
			}

			for (std::size_t i = 0; i < 4; ++i)
			{
				Coordinate next = { current.first + dx[i], current.second + dy[i] };
				if (next.first < 0 || next.first >= m_board.size() || next.second < 0 || next.second >= m_board[0].size())
				{
					continue; // Out of bounds
				}
				if (m_board[next.first][next.second] == '#' || visited[next.first][next.second]) {
					continue; // Wall or already visited
				}
				visited[next.first][next.second] = true;
				parent[next.first][next.second] = current;
				queue.push(next);
			}
		}

		// Reconstruct the path
		std::vector<Coordinate> path;
		Coordinate current = end;
		while (current != start) {
			path.push_back(current);
			current = parent[current.first][current.second];
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());
		return path[1];
	}
};