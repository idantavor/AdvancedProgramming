#include "BoardHandler.h"



bool BoardHandler::loadBoards(std::list<string> boardPaths) const
{
	bool boardIsOk = false;
	for (std::list<string>::iterator path = boardPaths.begin(); path != boardPaths.end(); ++path) {
		GameManager game();
		boardIsOk = game.loadBoard(path);
		if (boardIsOk) {
			break;
		}
	}

	return boardIsOk;
}
