#pragma once
#ifndef GAME_MANAGER
#define GAME_MANAGER
#include <string>
#include <list>
#include "Ship.h"
#include "UserFleet.h"
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "Utility.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include <winapifamily.h>
#include <functional>
#include <memory>
#include <vector>
#include "dirent.h"
#define BOARD_PATH 0
#define DLL_A_PATH 1
#define DLL_B_PATH 2
#define A_TURN 0
#define B_TURN 1
#define NUMBR_OF_SHIPS 5
#define WRONG_SIZE_A(c) cout << "Wrong size or shape for ship " << c << " for player A\n"
#define WRONG_SIZE_B(c) cout <<"Wrong size or shape for ship "  << c << " for player B\n"
#define TOO_MANY_A "Too many ships for player A\n"
#define TOO_FEW_A "Too few ships for player A\n"
#define TOO_MANY_B "Too many ships for player B\n"
#define TOO_FEW_B "Too few ships for player B\n"
#define ADJACENT "Adjacent Ships on Board\n"

using namespace std;

class GameManager {
public:
	int rowsSize = 0;
	int colsSize = 0;
	int depthSize = 0; 
	string** board;
	UserFleet fleetA;
	UserFleet fleetB;
	

	bool loadBoard(string boardPath);

	bool isKnownLetter(char c);

	bool validateBoard();

	void buildShip(int x, int y, int z, char shipChar, bool *** visitBoard, list<char>& failedCharA, list<char>& failedCharB);

	void shipCollectChars(int x, int y, int z, char shipChar, bool *** visitBoard, Ship & ship) const;

	bool isSpacesAreOK(int depth, int length, int width, char c) const;

	bool checkSpacesInPosition(int x, int y, int z, char c) const;


	void setAlgoA(IBattleshipGameAlgo* algo);
	void setAlgoB(IBattleshipGameAlgo* algo);
	IBattleshipGameAlgo* getAlgoA();
	IBattleshipGameAlgo* getAlgoB();

private:
	IBattleshipGameAlgo* playerAlgoA = nullptr;
	IBattleshipGameAlgo* playerAlgoB = nullptr;

};
#endif
