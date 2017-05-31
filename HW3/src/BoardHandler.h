#pragma once
#ifndef BOARD_HANDLER
#define BOARD_HANDLER
#include <string>
#include <list>
#include "GameManager.h"

using namespace std;

class BoardHandler {
public:
	BoardHandler(std::list<string> boardPaths) {
		this->boardPaths = boardPaths;
	}
	bool loadBoards(std::list<string> boardPaths) const;

	std::list<string> boardPaths;
};
#endif