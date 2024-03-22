#pragma once
#include "Abstract_Player.h"

class Human_Player final : public Abstract_Player
{
private:
	Coordinate _h_position;
	bool _is_alive;
public:
    Human_Player(const Coordinate& position = { 1,1 }) : _is_alive(true), _h_position(position) {}

	/// <summary>
	/// Changes player's position depends on pressed button
	/// </summary>
	void move() 
	{
		// TODO
		// Implement function, which changes position in board
	}

	// get_position
	// set_position

	bool is_alive() const
	{
		return _is_alive;
	}

	// Maybe TODO
	// Implement function, which changes _is_alive status of player
	void kill_player()
	{
		_is_alive = false;
	}
};


