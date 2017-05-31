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
	int boardWidth;
	int boardLength;
	int boardDepth;
	string* board;
	UserFleet userFleetA;
	UserFleet userFleetB;
	

	bool loadBoard(std::list<string> boardPath) const;

	bool isKnownLetter(char c);

	bool validateBoard();

	void buildShip(int x, int y, char shipChar, bool ** visitBoard, list<char>& failedCharA, list<char>& failedCharB);

	void shipCollectChars(int x, int y, char shipChar, bool ** visitBoard, Ship & ship) const;

	bool isSpacesAreOK(int i, int j, char c) const;

	bool checkSpacesInPosition(int x, int y, char c) const;

	void setAlgoA(IBattleshipGameAlgo* algo) {
		playerAlgoA = algo;
	}
	void setAlgoB(IBattleshipGameAlgo* algo) {
		playerAlgoB = algo;
	}

	IBattleshipGameAlgo* getAlgoA() {
		return playerAlgoA;
	}

	IBattleshipGameAlgo* getAlgoB() {
		return playerAlgoB;
	}


private:
	IBattleshipGameAlgo* playerAlgoA = nullptr;
	IBattleshipGameAlgo* playerAlgoB = nullptr;
	string* board;

};
#endif
