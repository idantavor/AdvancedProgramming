#pragma once
#include "Ship.h"
#include "UserFleet.h"
#ifndef BATTLE_MANAGER
#define BATTLE_MANAGER
#include "IBattleshipGameAlgo.h"
#include <string>
#include <vector>
#include <direct.h>
#include "GameData.h"



using namespace std;

class BattleManager {

	static IBattleshipGameAlgo* loadFromDLL(string path);

	public:
		~BattleManager();
		bool validateFilesExistance(const std::string & dirPath = "");
		void loadBoardsInToGameManager(std::list<string> boardPaths);
		//bool runBattle(const string& dirPath);

		std::list<GameData*> gamesList;
		std::list<string> dllFiels;

};
#endif

