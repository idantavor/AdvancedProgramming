#include "BattleManager.h"
#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include <winapifamily.h>
#include <functional>
#include <memory>
#include <vector>
#include "dirent.h"
#include "Ship.h"
#include <string>
#include "Utility.h"
#include <vector>
#include "AlgoDLL.h"
#include <unordered_set>
#include <queue>


boolean BattleManager::isPairDistinct(const std::vector<pair<int, int>> &v, const pair<int, int> &p) {
	for (const auto& pair : v) {
		if (pair.first == p.first || pair.second == p.first || pair.first == p.second || pair.second == p.second) {
			return false;
		}
	}
	return true;
}
boolean BattleManager::isPairDistinctInOne(const std::vector<pair<int, int>> &v, const pair<int, int> &p) {
	boolean first = true, second = true;
	for (const auto& pair : v) {
		if (pair.first == p.first || pair.second == p.first)  {
			first=false;
		}
		if (pair.first == p.second || pair.second == p.second) {
			second = false;
		}
	}
	return first || second;
}

void BattleManager::buildBattlesQueue()
{
	std::vector<pair<int, int>> combinations;
	//build all player combinations
	for (int i = 0; i < this->algorithms.size(); i++) {
		for (int j = 0; j < this->algorithms.size(); j++) {
			if (i == j)continue;
			combinations.push_back({ i,j });
		}
	}
	boolean isNumOfAlgsEven = (this->algorithms.size() % 2) ? false : true;
	int chunksize = isNumOfAlgsEven ? ((int)this->algorithms.size() / 2) : ((int)this->algorithms.size() / 2) + 1;
	//add to ThreadPool queue all of the battles in an even way
	while (combinations.size() != 0) {
		vector<pair<int, int>> chunk;
		if (combinations.size() < chunksize) {
			for (auto itr = combinations.begin(); itr != combinations.end();) {
				chunk.push_back(*itr);
				itr = combinations.erase(itr);
			}
		}
		else {
			while (chunk.size() < chunksize) {
				for (auto itr = combinations.begin(); itr != combinations.end();) {
					if (isPairDistinct(chunk, *itr) || (!isNumOfAlgsEven && chunk.size() == chunksize - 1 && isPairDistinctInOne(chunk, *itr))) {
						chunk.push_back(*itr);
						itr = combinations.erase(itr);
					}
					else itr++;
				}
			}
		}
		for (auto itr = this->gamesList.cbegin(); itr != gamesList.cend(); itr++) {
			for (const auto& alg_pair : chunk) {
				this->threadPool.addGameToQueue(&this->algorithms.at(alg_pair.first), &this->algorithms.at(alg_pair.second), *itr);
			}

		}
	}
}



BattleManager::BattleManager(string boardPaths,int numOfThreads)
{
	validateFilesExistance(boardPaths);
	//added const copy cnt'r in order to support vector insertion
	for (auto stringItr = this->dllFilePaths.begin(); stringItr != this->dllFilePaths.end(); stringItr++) {
		AlgoDLL algo(*stringItr, this->tRporter);
		if (algo.loadGetAlgFuncFromDLL(*stringItr)) {
			this->algorithms.push_back(algo);
		}
	}
	this->tRporter.setAlgNum(this->algorithms.size());
	this->threadNum = numOfThreads;
	this->buildBattlesQueue();
}

BattleManager::~BattleManager()
{
	for (auto &it : gamesList) delete it;
	gamesList.clear();
}

bool BattleManager::validateFilesExistance(const std::string& dirPath) {
	vector<string> errors = { "Missing board file (*.sboard) looking in path: ", "Missing an algorithm (dll) file looking in path: " };
	std::list<string> boardsFilesPath;
	bool isFilesFound = true;
	string searchIn;
	
	//check if dirPath provided
	if (dirPath.empty()) {
		searchIn = ".";
	}
	else {
		searchIn = dirPath;
	}
	
	//if dirPath provided check if dir exists
	if (searchIn != "." && !Utility::dirExists(searchIn)) {
		cout << "Wrong path : " << searchIn << endl;
		return false;
	}
	Logger logger = Logger("BattleManager");
	logger.setLogPath(searchIn);
	
	logger.Info("Start running program");
	
	//get file names in dir path
	vector<string> fileNames;
	Utility::getFileNamesFromDir(fileNames, searchIn);

	for (unsigned int i = 0; i < fileNames.size(); i++) {
		if (Utility::has_suffix(fileNames[i], "sboard")) {
			boardsFilesPath.push_back(searchIn + '\\' + fileNames[i]);
		}
		if (Utility::has_suffix(fileNames[i], "dll")) {
			dllFilePaths.push_back(searchIn + '\\' + fileNames[i]);
		}
	}

	if(boardsFilesPath.size() == 0)
	{
		cout << errors[0] << searchIn << endl;
		logger.Error(errors[0] + searchIn);
		isFilesFound = false;
	}

	if(dllFilePaths.size() <2 ){
		cout << errors[1] << searchIn << endl;
		logger.Error(errors[1] + searchIn);
		isFilesFound = false;
	}

	if (!isFilesFound) {
		return false;
	}

	// load boards 
	loadBoardsInToGameManager(boardsFilesPath);
	
	return true;
}

void BattleManager::loadBoardsInToGameManager(std::list<string> boardPaths) 
{

	bool boardIsOk = false;
	for (std::list<string>::iterator path = boardPaths.begin(); path != boardPaths.end(); ++path) {
		GameData* game = new GameData();
		boardIsOk = game->loadAndValidateBoard(*path);
		if (boardIsOk) {
			(gamesList).push_back(game);
		}
		else {
			delete game;
		}
	}
}

void BattleManager::runTournament()
{
	this->tRporter.startMonitor();
	this->threadPool.launchThreads(this->threadNum);
	this->threadPool.wait();
	this->tRporter.terminateMonitor();
}





	
