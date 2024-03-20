#pragma once
#include "Abstract_Player.h"

class Human_Player final : public Abstract_Player
{
public:
    Human_Player(const Coordinate& position = { 1,1 }) : _isAlive(true), _h_position(position) {}

	void move() 
	{
		// TODO
		// Implement function, which changes position in board
	}

	// getPosition
	// setPosition

	bool isAlive() const
	{
		return _isAlive;
	}

	// Maybe TODO
	// Implement function, which changes _isAlive status of player

private:
    Coordinate _h_position;
	bool _isAlive;
};


