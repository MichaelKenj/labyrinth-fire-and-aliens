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
void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

/// Main interface
void printInterface(GAME_MODE& game_mode);

void printChooseGameVariation(GAME_MODE& game_mode, bool& isModeSet)
{
	setcursor(0, 0);
	srand((unsigned)time(NULL));
	do {
		system("cls");
		gotoxy(10, 5); std::cout << " -------------------------------- ";
		gotoxy(10, 6); std::cout << " |        Choose game mode      | ";
		gotoxy(10, 7); std::cout << " --------------------------------";
		gotoxy(10, 9); std::cout << "1. Aliens";
		gotoxy(10, 10); std::cout << "2. Fire";
		gotoxy(10, 11); std::cout << "3. Back to main menu";
		gotoxy(10, 12); std::cout << "Select option: ";

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

void printInterface(GAME_MODE& game_mode)
{
	bool isModeSet = false;
	do {
		setcursor(0, 0);
		srand((unsigned)time(NULL));

		system("cls");
		gotoxy(10, 5); std::cout << " -------------------------------- ";
		gotoxy(10, 6); std::cout << " |  Labyrinth: Fire and Aliens  | ";
		gotoxy(10, 7); std::cout << " --------------------------------";
		gotoxy(10, 9); std::cout << "1. Play";
		gotoxy(10, 10); std::cout << "2. Quit";
		gotoxy(10, 11); std::cout << "Select option: ";
		char menu_op = _getche();

		if (menu_op == '1')
		{
			printChooseGameVariation(game_mode, isModeSet);
		}
		else if (menu_op == '2')
			exit(0);

		if (isModeSet)
		{
			return;
		}

	} while (true);
}