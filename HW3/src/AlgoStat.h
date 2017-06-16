#pragma once
#include <string>
using namespace std;
class AlgoStat {
public:
	int wins = 0;
	int losses = 0;
	int totalGames = 0;
	int pointsFor = 0;
	int pointsAgainst = 0;
	string algName;
	
	AlgoStat(string _algName): algName(_algName) {
	}

	AlgoStat() {
	}
	
	void setAlgName(string name) {
		algName = name;
	}
};