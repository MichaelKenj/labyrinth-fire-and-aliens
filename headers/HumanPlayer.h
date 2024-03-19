#pragma once
#include "Abstract_Player.h"



class Human_Player : public Abstract_Player
{
public:
	void move() 
	{

	}

	// getPosition
	// setPosition

	bool isAlive() const
	{
		return _isAlive;
	}

	

private:
	Coordinate h_position;
	bool _isAlive;
};


