#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include "AlgoDLL.h"
#include "GameData.h"
#include "IBattleshipGameAlgo.h"
using namespace std;

class BattleThreadPool {

private:
	int threadCnt = 0;
	ConcurentQueue<tuple<AlgoDLL*, AlgoDLL*, GameData*>> gamesQueue;
	bool runBattle(AlgoDLL* p1, AlgoDLL* p2, GameData* bd);
	vector<unique_ptr<thread>> threads;
	bool stopThreads = false;

public:
	BattleThreadPool() {

	};

	void launchThreads(int threadNum) {
		threadCnt = threadNum;
		for (int i = 0; i < threadNum; i++) {
			threads.push_back(unique_ptr<thread>{ new thread(&BattleThreadPool::HandleBattleQueue, this)});
		}
	}

	void wait() {
		for (auto itr = this->threads.begin(); itr != threads.end(); itr++) {
			(**itr).join();
		}
	}
	
	void HandleBattleQueue() {
		while (!this->stopThreads) {
		};
	}

	void addGameToQueue(AlgoDLL* p1, AlgoDLL* p2, GameData* gameData) {
		this->gamesQueue.push(make_tuple(p1, p2, gameData));
	}



};
