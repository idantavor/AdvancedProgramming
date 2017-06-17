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
#include <unordered_map>
#include <queue>




void BattleManager::buildBattlesQueue()
{
	std::unordered_map<int, vector<pair<int, int>>> combinationMap;
	//build all player combinations
	for (size_t  i = 0; i<this->algorithms.size(); i ++ ) {
		for (size_t  j = 0; j < this->algorithms.size(); j++) {
			if (i == j)continue;
			if (combinationMap.find(i)!=combinationMap.end()) {
				combinationMap.find(i)->second.push_back({ i,j });
			}
			else {
				std::vector<pair<int,int>> v;
				v.push_back({ i,j });
				combinationMap.insert({ i,v });
			}
		}
	}
	//add to ThreadPool queue all of the battles in an even way
	for (auto itr = this->gamesList.cbegin(); itr != gamesList.cend(); itr++) {
		for (size_t  i = 0; i < this->algorithms.size() - 1; i++) {// i will be each dll combination index
			for (size_t  j = 0; j < this->algorithms.size(); j++) {
				pair<int, int> combination = combinationMap.find(j)->second.at(i);
				this->threadPool.addGameToQueue(&this->algorithms.at(combination.first), &this->algorithms.at(combination.second), *itr);
			}
		}

	}
	
}

BattleManager::BattleManager(string boardPaths,int numOfThreads)
{
	validateFilesExistance(boardPaths);
	//added const copy cnt'r in order to support vector insertion
	for (auto stringItr = this->dllFilePaths.begin(); stringItr != this->dllFilePaths.end(); stringItr++) {
		AlgoDLL alg(*stringItr, this->tRporter);
		this->algorithms.push_back(alg);
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
		isFilesFound = false;
	}

	/*
	if(dllFiels.size() <2 ){
		cout << errors[1] << searchIn << endl;
		isFilesFound = false;
	}

	*/
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





	
