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

	void printHeader() {
		printElement("#", numWidth);
		printElement("Team Name", nameWidth);
		printElement("wins", numWidth);
		printElement("losses", numWidth);
		printElement("%", numWidth);
		printElement("Points For", pointWidth);
		printElement("Points Against", pointWidth);
		cout << endl << endl;
	}
	void printStat(AlgoStat& stat,int number) {
		printElement(std::to_string(number)+'.', numWidth);
		printElement(stat.algName, nameWidth);
		printElement(stat.wins, numWidth);
		printElement(stat.losses, numWidth);
		float percentage = (stat.totalGames == 0) ? 0 : (float(stat.wins) / (stat.wins + stat.losses)) * 100;
		cout << setprecision(2)<<fixed;
		printElement(percentage, numWidth);
		printElement(stat.pointsFor, pointWidth);
		printElement(stat.pointsAgainst, pointWidth);
		cout << endl<<endl;
	}
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
	void test() {
		printHeader();
		AlgoStat alg("idan the king");
		alg.wins = 1000;
		alg.losses = 2;
		alg.totalGames = 1002;
		alg.pointsFor = 2;
		alg.pointsAgainst = 3;
		printStat(alg,100);
	
	}

	
};