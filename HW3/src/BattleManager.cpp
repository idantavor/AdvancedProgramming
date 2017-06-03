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




IBattleshipGameAlgo* BattleManager::loadFromDLL(string path)
{	
	UINT oldMode = SetErrorMode(0);
	SetErrorMode(oldMode | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
		IBattleshipGameAlgo * instance;
		HINSTANCE hDll;
		try {
		// Load dynamic library 
		hDll = LoadLibraryA(path.c_str());
		using FunctionPtr = IBattleshipGameAlgo* (*) ();
		if (hDll)
		{
			// GetAlgorithm function
			auto GetAlgorithm = reinterpret_cast<FunctionPtr>(GetProcAddress(hDll, "GetAlgorithm"));
			instance = GetAlgorithm();
		}
		else {
			throw exception("failed to load library");
		}
	}
	catch (exception e) {
		throw e;
	}
	
	return instance;
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
			dllFiels.push_back(searchIn + '\\' + fileNames[i]);
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





	
