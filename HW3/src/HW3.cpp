#include <string>
#include <iostream>
#include "logger.h"
#include "ConcurentQueue.h"
#include "BattleThreadPool.h"
#include "TournamentReporter.h"
#include "BattleManager.h"
#include "Utility.h"
using namespace std;


void main(int argc,char*argv[]) {
	
	// get command line path
	string path = ".";
	if (argc > 1) {
		if (argv[1][0] != '-') {
			path = argv[1];
		}
	}

	// get thread number - default is 4
	auto result = Utility::getCommandLineArg("threads", argc, argv);
	int threads = 4;
	if (result.first) {
		try {
			threads = (int(stoi(result.second)));
		}
		catch (exception e) {

		}
	};
	BattleManager bm(path,threads);
	bm.runTournament();
};


