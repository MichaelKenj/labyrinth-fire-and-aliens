#include <iostream>
#include "headers/Game.h"
#include "headers/printHelperMethods.h"

int main()
{
	/*Board bord(20, std::vector<char>(20,' '));
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			if (j == 0 || j == 19)
			{
				bord[i][j] = '#';
			}
			else
			{
				bord[i][j] = '.';
			}
		}
	}
	print(bord);*/

	Game game;
	GAME_MODE game_mode;

	/// Printing menu interface
	printInterface(game_mode);


	/// Starting game logic
	game.start();
	game.play();//playi nersic pti kanchvi stop
	//stopi nersic pti lini showSolution



}