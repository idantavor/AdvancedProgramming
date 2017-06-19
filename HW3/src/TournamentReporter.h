#pragma once
#include "ConcurentQueue.h"
#include "AlgoStat.h"
#include <unordered_map>
#include <vector>
#include <thread>
#include <iomanip>
#include <iostream>
using namespace std;
class TournamentReporter {
private:
	const char separator = ' ';
	const int nameWidth = 20;
	const int numWidth = 10;
	const int pointWidth = 14;
	ConcurentQueue<AlgoStat> statQueue;
	unordered_map<int,vector<AlgoStat>> roundToStatMap;
	unique_ptr<thread> monitorThread;
	bool stopMonitor = false;
	size_t algNum = 0;
	void handleQueue();
	void report(vector<AlgoStat> & algStatList);
	template<typename T> void printElement(T t, const int& width)
	{
		cout << left << setw(width) << setfill(separator) << t;
	}

	void printHeader(); 
	void printStat(AlgoStat& stat, int number); 
public:
	TournamentReporter(size_t _algNum) :algNum(_algNum) {};
	
	TournamentReporter() {};

	void setAlgNum(size_t _algNum) {
		algNum = _algNum;
	}
	void notifyStat(const AlgoStat stat) {
		this->statQueue.push(stat);
	}

	void startMonitor() {
		this->monitorThread = unique_ptr<thread>{ new thread(&TournamentReporter::handleQueue, this) };
	}

	void terminateMonitor() {
		this->stopMonitor = true;
		this->monitorThread->join();
	}


	
};