// hw1.cpp : Defines the entry point for the console application.
//

#include "BattleManager.h"
#include <stdio.h>
#include "Utility.h"
#include "GameData.h"

int main(int argc, char* argv[])
{
	string path;
	if (argc > 1) {
		path = argv[1][0] != '-' ? argv[1] : "";//get path parameter
	}

	BattleManager bm = BattleManager();
	if (!bm.validateFilesExistance(path)) {
		return -1;
	}
	return 0;
}


