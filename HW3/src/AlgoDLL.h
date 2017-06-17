#pragma once
#include "IBattleshipGameAlgo.h"
#include <string>
#include <shlwapi.h>
#include <iostream>
#include <mutex>
#include "AlgoStat.h"
#include "TournamentReporter.h"
#include "logger.h"

using namespace std;
using GetAlgoFunctionPtr = IBattleshipGameAlgo* (*) ();

class AlgoDLL {

private:
	mutex editLock;
	GetAlgoFunctionPtr getAlgoFunc;
	string algName;
	AlgoStat algStat;
	TournamentReporter &tReporter;

public:
	AlgoDLL(string dll_path,TournamentReporter &_tReporter):tReporter(_tReporter) {
			algName = AlgoDLL::GetFileNameFromPath(dll_path);
			algStat.setAlgName(algName);
	}
	
	AlgoDLL(const AlgoDLL &a):tReporter(a.tReporter) {
		this->getAlgoFunc = a.getAlgoFunc;
		this->algName = a.algName;

	};
	AlgoDLL& operator =(const AlgoDLL&) = delete;
	auto GetAlgoInstance() {
		return this->getAlgoFunc();
	}

	static string GetFileNameFromPath(const string &path) {
		size_t lastSlashIndex = path.find_last_of('\\');
		return path.substr(lastSlashIndex + 1);
	}
	
	void  addWin() {
		std::lock_guard<std::mutex> lock(editLock);
		this->algStat.wins += 1;
		this->incGameCnt();
	}

	void incGameCnt() {
		this->algStat.totalGames+= 1;
		tReporter.notifyStat(this->algStat);
		
	}

	void addLose() {
		std::lock_guard<std::mutex> lock(editLock);
		this->algStat.losses += 1;
		this->incGameCnt();
	}

	bool loadGetAlgFuncFromDLL(string path);
};