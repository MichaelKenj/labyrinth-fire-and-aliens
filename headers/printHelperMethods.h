#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>  
#include <dos.h>   
#include <windows.h>  
#include <time.h>  
#include <limits>
#include <iostream>
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

void ClearWindow()
{
    COORD cursorPosition; 
    cursorPosition.X = 0; 
    cursorPosition.Y = 0; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
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

void printIsPlayerWantsToShowSolutionFrame()
{
    setCursor(0, 0);
    srand((unsigned)time(NULL));
    system("cls");
    goToXY(10, 5); std::cout << " +----------------------------------+ ";
    goToXY(10, 6); std::cout << " | Do you want to see the solution? | ";
    goToXY(10, 7); std::cout << " +----------------------------------+ ";
    goToXY(10, 9); std::cout << "1. Yes";
    goToXY(10, 10); std::cout << "2. No";
    goToXY(10, 11); std::cout << "Select option: ";
}

void printCoordinate(Coordinate cord)
{
	std::cout << "(" << cord.first << "," << cord.second << ") ";
}

///-----------PRINT FRAMES OF TITLES------------

// _hell is a vector of rows of big title
void printFrame(const std::vector<std::string>& _hell)
{
    std::cout << "\n\n\n\n\n\n\n\n\n\n";
    for (int i = 0; i < _hell.size(); ++i)
    {
        std::cout << _hell[i] << '\n';
    }

}
///-----------FOR HELLO------------

// Sliding Hello
void slideGameOver()
{  
    std::string row1 = " ±±±±  ±±±  ±   ± ±±±±±       ±±±  ±   ± ±±±±± ±±±± ";
    std::string row2 = "±     ±   ± ±± ±± ±          ±   ± ±   ± ±     ±   ±";
    std::string row3 = "±  ±± ±±±±± ± ± ± ±±±±       ±   ±  ± ±  ±±±±  ±±±± ";
    std::string row4 = "±   ± ±   ± ±   ± ±          ±   ±  ± ±  ±     ±   ±";
    std::string row5 = "±±±±  ±   ± ±   ± ±±±±±       ±±±    ±   ±±±±± ±   ±";
    std::vector<std::string> _hello{ row1, row2, row3, row4, row5};
    int spaces = 10;

    std::string sps = "";
    for (int i = 0; i < 50; ++i)
        sps += " ";
    for (std::size_t i = 0; i < _hello.size(); ++i)
    {
        _hello[i] = sps + _hello[i];
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
    while (spaces >= 0)
    {
        printFrame(_hello);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        --spaces;
        if (spaces == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        system("cls");
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
        for (std::size_t i = 0; i < _hello.size(); ++i)
        {
            _hello[i].erase(0, 1);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void slideWin()
{
    std::string row1 = "±±±           ±±±   ±±±   ±±±   ±±± ";
    std::string row2 = " ±±±         ±±±    ±±±   ±±±±  ±±± ";
    std::string row3 = "  ±±± ±±±±± ±±±     ±±±   ±±±±±±±±± ";
    std::string row4 = "   ±±±±± ±±±±±      ±±±   ±±± ±±±±± ";
    std::string row5 = "    ±±±   ±±±       ±±±   ±±±   ±±± ";
    std::vector<std::string> _hello{ row1, row2, row3, row4, row5 };
    int spaces = 10;

    std::string sps = "";
    for (int i = 0; i < 50; ++i)
        sps += " ";
    for (std::size_t i = 0; i < _hello.size(); ++i)
    {
        _hello[i] = sps + _hello[i];
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    while (spaces >= 0)
    {
        printFrame(_hello);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        --spaces;
        if (spaces == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        system("cls");
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
        for (std::size_t i = 0; i < _hello.size(); ++i)
        {
            _hello[i].erase(0, 1);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}
///-----------FOR MENU-------------