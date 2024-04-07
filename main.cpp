#include "headers/Game.h"

int main()
{
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 900, 900, TRUE);
	while (true) 
	{
		Game game;
		game.play();
	}
	return 0;
}