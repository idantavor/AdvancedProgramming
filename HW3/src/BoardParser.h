#pragma once
#ifndef BOARD_PARSER
#define BOARD_PARSER
#include <string>
#include <fstream>
#include "dirent.h"
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include <vector>
#define SPACE ' '
#include <iostream>
#include <ctype.h>


using namespace std;

class BoardParser {
public:
	
	BoardParser(string boardPath);
	string** loadBoard();
	unsigned int getRowSize();
	unsigned int getDepthSize();
	unsigned int getColSize();

private:

	string getNextLine(streambuf * borderStream);

	void addRowsToBoard(int depth, int countRowsInBord);

	void addTableToBoard(int depth);

	void addLineToBoard(string & line, int depth, int countRowsInBord);

	void initializeBoard();

	bool addCharToString(string & line, char c);

	bool handleSizesLine(streambuf * borderStream);

	bool isKnownLetter(char c);

	unsigned int rowsSize = 0;
	unsigned int colsSize = 0;
	unsigned int depthSize = 0;

	bool endOfFile = false;
	string** board = nullptr;
	string boardPath;
};
#endif
