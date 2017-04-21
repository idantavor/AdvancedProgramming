// hw1.cpp : Defines the entry point for the console application.
//

#include "BattleManager.h"
#include "BattleShipGameFromFile.h"
#include <iostream>
#include <cstdio>
int main(int argc, char* argv[])
{	
	string path;
	if (argc > 1) {
		path = argv[1];
	}
	BattleManager bm;
	if (!bm.runBattle(path)) {
		return -1;
	}
	return 0;
}

