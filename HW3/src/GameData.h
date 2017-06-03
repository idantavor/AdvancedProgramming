#pragma once
#ifndef GAME_DATA
#define GAME_DATA
#include <string>
#include <list>
#include "Ship.h"
#include "UserFleet.h"
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "Utility.h"
#include "BoardParser.h"
#include <functional>
#include <memory>
#include "dirent.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#define A_TURN 0
#define B_TURN 1
#define WRONG_SIZE_A(c) cout << "Wrong size or shape for ship " << c << " for player A\n"
#define WRONG_SIZE_B(c) cout <<"Wrong size or shape for ship "  << c << " for player B\n"
#define TOO_MANY_A "Too many ships for player A\n"
#define TOO_FEW_A "Too few ships for player A\n"
#define TOO_MANY_B "Too many ships for player B\n"
#define TOO_FEW_B "Too few ships for player B\n"
#define ADJACENT "Adjacent Ships on Board\n"
#define SPACE ' '


using namespace std;

class GameData {
public:
	unsigned int rowsSize = 0;
	unsigned int colsSize = 0;
	unsigned int depthSize = 0;
	string** board = nullptr;
	UserFleet fleetA;
	UserFleet fleetB;
	void clone(GameData& cloned);
	GameData() = default;
	~GameData();
	void setAlgoA(IBattleshipGameAlgo* algo);
	void setAlgoB(IBattleshipGameAlgo* algo);
	IBattleshipGameAlgo* getAlgoA();
	IBattleshipGameAlgo* getAlgoB();

	bool loadAndValidateBoard(string boardPath);

private:
	
	void buildShip(int x, int y, int z, char shipChar, bool *** visitBoard, list<char>& failedCharA, list<char>& failedCharB);

	void shipCollectChars(int x, int y, int z, char shipChar, bool *** visitBoard, Ship & ship) const;

	bool isSpacesAreOK(int depth, int length, int width, char c) const;

	bool checkSpacesInPosition(int x, int y, int z, char c) const;

	IBattleshipGameAlgo* playerAlgoA = nullptr;
	IBattleshipGameAlgo* playerAlgoB = nullptr;

};
#endif
