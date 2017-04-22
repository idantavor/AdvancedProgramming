// hw1.cpp : Defines the entry point for the console application.
//

#include "BattleManager.h"
#include "BattleShipGameFromFile.h"
#include <iostream>
#include <cstdio>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "ConsolePrinter.h"
#include "Utility.h"

int main(int argc, char* argv[])
{	
	string path;
	if (argc > 1) {
		path = argv[1][0]!='-'?argv[1]:"";//get path parameter
	}

	int delay=ATTACKS_DELAY;
	auto result = Utility::getCommandLineArg("quiet", argc, argv); //get quiet param
	bool quiet = result.first;

	result=Utility::getCommandLineArg("dealy", argc, argv);//get delay param
	if (result.first) {
		if (result.second != "") {
			try {
				delay = atoi(result.second.c_str()) * 1000;
			}
			catch (exception) {
				
			}
		}
	}

	BattleManager bm=BattleManager(10,quiet,delay);
	if (!bm.runBattle(path)) {
		return -1;
	}
	return 0;
}


