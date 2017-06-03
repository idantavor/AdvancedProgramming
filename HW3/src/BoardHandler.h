#pragma once
#ifndef BOARD_HANDLER
#define BOARD_HANDLER
#include <string>
#include <list>
#include "GameManager.h"

using namespace std;

class BoardHandler {
public:
	std::list<GameManager*> loadBoards(std::list<string> boardPaths) const;

};
#endif