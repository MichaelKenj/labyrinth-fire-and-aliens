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

void goToXY(int x, int y) 
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setCursor(bool visible, DWORD size) 
{
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void printMainMenuFrame()
{
	setCursor(0, 0);
	srand((unsigned)time(NULL));
	system("cls");
	goToXY(10, 5); std::cout << " +------------------------------+ ";
	goToXY(10, 6); std::cout << " |  Labyrinth: Fire and Aliens  | ";
	goToXY(10, 7); std::cout << " +------------------------------+ ";
	goToXY(10, 9); std::cout << "1. Play";
	goToXY(10, 10); std::cout << "2. Quit";
	goToXY(10, 11); std::cout << "Select option: ";
}

void printChooseGameModeFrame()
{
	setCursor(0, 0);
	srand((unsigned)time(NULL));
	system("cls");
	goToXY(10, 5); std::cout << " +------------------------------+ ";
	goToXY(10, 6); std::cout << " |        Choose game mode      | ";
	goToXY(10, 7); std::cout << " +------------------------------+ ";
	goToXY(10, 9); std::cout << "1. Fire";
	goToXY(10, 10); std::cout << "2. Aliens";
	goToXY(10, 11); std::cout << "3. Back to main menu";
	goToXY(10, 12); std::cout << "Select option: ";
}