#pragma once
#include "Ship.h"
#include "UserFleet.h"
#ifndef USER_GAME_DATA
#define USER_GAME_DATA
#include "IBattleshipGameAlgo.h"
#include <string>
#include <vector>
#include <direct.h>
#include "UserFleet.h"
#define PLAYER_A 0
#define PLAYER_B 1
#define SPACE ' '



using namespace std;

class UserGameData : public BoardData
{
	public:
		UserGameData(int playerNumber, string** board);
		char charAt(Coordinate c) const override;  //returns only selected players' chars
		
	private:
		string** board;
		int palyerNumber;

};
#endif

