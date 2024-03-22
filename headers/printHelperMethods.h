#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>  
#include <dos.h>   
#include <windows.h>  
#include <time.h>  
#include <limits>
#include "Game_Mode.h"



HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void go_to_xy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void set_cursor(bool visible, DWORD size) {
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

/// Main interface
void print_interface(GAME_MODE& game_mode);

void print_choose_game_variation(GAME_MODE& game_mode, bool& isModeSet)
{
	set_cursor(0, 0);
	srand((unsigned)time(NULL));
	do {
		system("cls");
		go_to_xy(10, 5); std::cout << " -------------------------------- ";
		go_to_xy(10, 6); std::cout << " |        Choose game mode      | ";
		go_to_xy(10, 7); std::cout << " --------------------------------";
		go_to_xy(10, 9); std::cout << "1. Aliens";
		go_to_xy(10, 10); std::cout << "2. Fire";
		go_to_xy(10, 11); std::cout << "3. Back to main menu";
		go_to_xy(10, 12); std::cout << "Select option: ";

		char choose_op;
		choose_op = _getche();
		if (choose_op == '1')
		{
			game_mode = ALIEN;
			isModeSet = true;
			return;
		}
		else if (choose_op == '2')
		{
			game_mode = FIRE;
			isModeSet = true;
			return;
		}
		else if (choose_op == '3')
		{
			isModeSet = false;
			return;
		}

	} while (true);
}

void print_interface(GAME_MODE& game_mode)
{
	bool isModeSet = false;
	do {
		set_cursor(0, 0);
		srand((unsigned)time(NULL));

		system("cls");
		go_to_xy(10, 5); std::cout << " -------------------------------- ";
		go_to_xy(10, 6); std::cout << " |  Labyrinth: Fire and Aliens  | ";
		go_to_xy(10, 7); std::cout << " --------------------------------";
		go_to_xy(10, 9); std::cout << "1. Play";
		go_to_xy(10, 10); std::cout << "2. Quit";
		go_to_xy(10, 11); std::cout << "Select option: ";
		char menu_op = _getche();

		if (menu_op == '1')
		{
			print_choose_game_variation(game_mode, isModeSet);
		}
		else if (menu_op == '2')
			exit(0);

		if (isModeSet)
		{
			return;
		}

	} while (true);
}