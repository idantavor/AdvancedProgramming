#include <string>
#include <iostream>
#include "logger.h"
#include "ConcurentQueue.h"
#include "BattleThreadPool.h"
#include "TournamentReporter.h"
#include "BattleManager.h"
#include "Utility.h"
using namespace std;

static ostream& operator<<(ostream &s,vector<std::string> &v) {
	for (auto itr = v.begin(); itr != v.end(); itr++) {
		if ((itr+1)!=v.end())
			s << *itr <<',';
		else s << *itr << std::endl;
	}
	return s;
}
static ostream& operator<<(ostream &s, vector<tuple<std::string, std::string, std::string>> &v) {
	for (auto itr = v.begin(); itr != v.end(); itr++) {
		auto t = *itr;
		if ((itr + 1) != v.end())
			s << '('<<std::get<0>(t)<<","<<std::get<1>(t) << "," <<std::get<2>(t)<<')' << ",";
		else s << '(' << std::get<0>(t) << ',' << std::get<1>(t) << ',' << std::get<2>(t) << ')';
	}
	return s;
}
void logicTest(vector<std::string> &dlls,vector<std::string> &games) {
	std::cout << "dlls:" << std::endl << dlls << std::endl ;
	std::cout << "games:"<<std::endl<<games << std::endl ;
	std::unordered_map<int, vector<pair<int, int>>> combinationMap;
	std::vector<std::tuple<std::string, std::string, std::string>> result;
	//build all player combinations
	for (int i = 0; i<dlls.size(); i++) {
		for (int j = 0; j < dlls.size(); j++) {
			if (i == j)continue;
			if (combinationMap.find(i) != combinationMap.end()) {
				combinationMap.find(i)->second.push_back({ i,j });
			}
			else {
				std::vector<pair<int, int>> v;
				v.push_back({ i,j });
				combinationMap.insert({ i,v });
			}
		}
	}
	//add to ThreadPool queue all of the battles in an even way
	for (auto itr = games.cbegin(); itr != games.cend(); itr++) {
		for (int i = 0; i < dlls.size() - 1; i++) {// i will be each dll combination index
			for (int j = 0; j < dlls.size(); j++) {
				pair<int, int> combination = combinationMap.find(j)->second.at(i);
				result.push_back(std::tuple<std::string,std::string,std::string>{ dlls.at(combination.first),dlls.at(combination.second), *itr });
			}
		}

	}
	cout << "result:" << endl << result<<endl;
}
void main(int argc,char*argv[]) {
	
	auto result = Utility::getCommandLineArg("threads", argc, argv);
	int threads = 4;
	if (result.first) {
		try {
			threads = (int(stoi(result.second)));
		}
		catch (exception e) {

		}
	};
	BattleManager bm(".",threads);
	
};

