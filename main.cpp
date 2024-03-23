#include "headers/Game.h"
#include <chrono>

/// <summary>
/// Test temprorary function for spreadFire function work
/// </summary>
void spreadFireTest()
{
	Game game;
	int i = 120;
	do 
	{
		system("cls");
		game.getLabyrinth().spreadFire();
		game.getLabyrinth().printBoard();
		Sleep(50);
	} while (i--);
}

int main()
{
	Game game;
	game.play();
	return 0;
}