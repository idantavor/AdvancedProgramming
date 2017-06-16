#include "UserGameData.h"

UserGameData::UserGameData(int playerNumber, string** board)
{
	this->palyerNumber = palyerNumber;
	this->board = board;
}

char UserGameData::charAt(Coordinate c) const
{
	char cShip = board[c.depth- -1 ][c.row - 1][c.col - 1];

	if ((palyerNumber == PLAYER_A && cShip >= 'a' && cShip <= 'z')
		|| palyerNumber == PLAYER_B && cShip >= 'A' && cShip <= 'Z') {
		return cShip;
	}

	return SPACE;
}
