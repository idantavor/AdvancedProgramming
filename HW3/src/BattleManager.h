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
#include "TournamentReporter.h"
#include "BattleThreadPool.h"
#include "logger.h"



using namespace std;

class BattleManager {

private:
	TournamentReporter tRporter;
	BattleThreadPool threadPool;
	vector<AlgoDLL> algorithms;
	int threadNum = 0;
	void buildBattlesQueue();
public:
	BattleManager(string boardPaths, int numOfThreads = 4);
	~BattleManager();
	bool validateFilesExistance(const std::string & dirPath = "");
	void loadBoardsInToGameManager(std::list<string> boardPaths);
	boolean isPairDistinct(const std::vector<pair<int, int>> &v, const pair<int, int> &p);
	boolean isPairDistinctInOne(const std::vector<pair<int, int>> &v, const pair<int, int> &p);
	std::list<GameData*> gamesList;
	std::list<string> dllFilePaths;
	void runTournament();

};
#endif

