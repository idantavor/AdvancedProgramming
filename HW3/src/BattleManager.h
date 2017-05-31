#pragma once
#include "Ship.h"
#include "UserFleet.h"
#ifndef BATTLE_MANAGER
#define BATTLE_MANAGER
#include "IBattleshipGameAlgo.h"
#include <string>
#include <vector>
#include <direct.h>


using namespace std;

class BattleManager {
		vector<string> filePaths;

	static IBattleshipGameAlgo* loadFromDLL(string path);

	public:
		~BattleManager() {

		}
		BattleManager()
		{
			
		}


	bool validateFilesExistanceAndBoardValidate(const std::string& dirPath = "");
	bool runBattle(const string& dirPath);
	void deleteBoard(char** boardToDelete) const;
};
#endif

