#pragma once
#ifndef SHARED_BOARD
#define SHARED_BOARD
#include <string>
#include <fstream>
#include <memory>
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

class SharedBoard {
public:

	SharedBoard(string boardPath);
	~SharedBoard();
	bool loadBoard();
	unsigned int  getRowSize();
	unsigned int  getDepthSize();
	unsigned int  getColSize();
	char getCharAt(int x, int y, int z);

private:

	string getNextLine(streambuf * borderStream);

	void addRowsToBoard(unsigned int depth, unsigned int countRowsInBord);

	void addTableToBoard(unsigned int depth);

	void addLineToBoard(string & line, unsigned int depth, unsigned int countRowsInBord);

	void initializeBoard();

	bool addCharToString(string & line, char c);

	bool handleSizesLine(streambuf * borderStream);

	bool isKnownLetter(char c);

	unsigned int  rowsSize = 0;
	unsigned int  colsSize = 0;
	unsigned int  depthSize = 0;

	bool endOfFile = false;
	string** board;
	string boardPath;
};
#endif
