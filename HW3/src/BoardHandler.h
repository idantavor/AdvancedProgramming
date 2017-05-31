#pragma once
#ifndef BOARD_HANDLER
#define BOARD_HANDLER
#include <string>
#include <list>
using namespace std;

class BoardHandler {
public:
	BoardHandler(string* board) {
		this->board = board;
	}
	bool loadBoard(std::list<string> boardsPath) const;

private:
	bool checkAndLoadBoard(const string& boardPath) const;
	string* board;

};
#endif