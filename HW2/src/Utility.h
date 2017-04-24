#pragma once

using namespace std;
#include "UserFleet.h"
#include <string>

class Utility{
public:
	static UserFleet* setUserFleetFromBoard(const char** board, int numRows, int numCols);
	static void buildUserShip(const char** board, int x, int y, char shipChar, bool** visitBoard, std::list<Ship*>* shipsList, int numRows, int numCols);
	static void collectShipChars(const char** board, int x, int y, char shipChar, bool** visitBoard, list<Position>* positionList, int numRows, int numCols);
	static pair<bool,string> getCommandLineArg(const string arg_to_find, int argc, char* argv[]);
};


