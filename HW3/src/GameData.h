#pragma once
#ifndef GAME_DATA
#define GAME_DATA
#include <string>
#include <list>
#include "Ship.h"
#include "UserFleet.h"
#include "InflatableBoat.h"
#include "SharedBOard.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "Utility.h"
#include "Logger.h"
#include <functional>
#include <memory>
#include "dirent.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#define A_TURN 0
#define B_TURN 1
#define WRONG_SIZE_A(c, path) Logger("GameData").Warning("Wrong size or shape for ship " + string(1,c) + " for player A, in board: " + path)
#define WRONG_SIZE_B(c, path) Logger("GameData").Warning("Wrong size or shape for ship " + string(1,c) + " for player B, in board: " + path)
#define ADJACENT(path) "Adjacent Ships on Board: " + path
#define MORE_SHIP_A(a, b, path) Logger("GameData").Warning("Player A as more ships then player B, (Player A: " +  to_string(a) + " ,Player B: " + to_string(b) + ") in board: " + path)
#define MORE_SHIP_B(a, b, path) Logger("GameData").Warning("Player B as more ships then player A, (Player A: " +  to_string(a) + " ,Player B: " + to_string(b) + ") in board: " + path)
#define DIFFERENT_SHIPS(path) Logger("GameData").Warning("Players have different ships, in board: " + path)

#define SPACE ' '


using namespace std;

class GameData {
public:
	std::shared_ptr<SharedBoard> board;
	UserFleet fleetA;
	UserFleet fleetB;
	void clone(GameData& cloned);
	GameData() = default;
	void setAlgoA(IBattleshipGameAlgo* algo);
	void setAlgoB(IBattleshipGameAlgo* algo);
	IBattleshipGameAlgo* getAlgoA();
	IBattleshipGameAlgo* getAlgoB();
	void setUserFleetA(UserFleet& fleetA);
	void setUserFleetB(UserFleet& fleetB);
	bool loadAndValidateBoard(string boardPath);
private:
	
	void buildShip(unsigned int x, unsigned int y, unsigned int z, char shipChar, bool *** visitBoard, list<char>& failedCharA, list<char>& failedCharB);

	void shipCollectChars(unsigned int x, unsigned int y, unsigned int z, char shipChar, bool *** visitBoard, Ship & ship) const;

	bool isSpacesAreOK(unsigned int depth, unsigned int length, unsigned int width, char c) const;

	bool checkSpacesInPosition(int x, int y, int z, char c) const;
	IBattleshipGameAlgo* playerAlgoA = nullptr;
	IBattleshipGameAlgo* playerAlgoB = nullptr;

};
#endif
