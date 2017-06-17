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

	bool checkSpacesInPosition(size_t x, size_t y, size_t z, char c) const;
	IBattleshipGameAlgo* playerAlgoA = nullptr;
	IBattleshipGameAlgo* playerAlgoB = nullptr;

};
#endif
