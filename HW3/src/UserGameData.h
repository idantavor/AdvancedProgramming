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


using namespace std;

class UserGameData : public BoardData
{
	public:
		UserFleet fleet;
		~UserGameData() = default;
		char charAt(Coordinate c) const = 0;  //returns only selected players' chars
};
#endif

