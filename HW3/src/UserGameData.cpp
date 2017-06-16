#include "UserGameData.h"

UserGameData::UserGameData(int playerNumber, shared_ptr<SharedBoard> board)
{
	this->palyerNumber = palyerNumber;
	this->board = board;
}

char UserGameData::charAt(Coordinate c) const
{
	char cShip = board->getCharAt(c.row - 1, c.col - 1, c.depth - -1); 

	if ((palyerNumber == PLAYER_A && cShip >= 'a' && cShip <= 'z')
		|| palyerNumber == PLAYER_B && cShip >= 'A' && cShip <= 'Z') {
		return cShip;
	}

	return SPACE;
}
