
#include "battleship.h"

char battleship::symbolAPlayer = 'D';
char battleship::symbolBPlayer = 'd';
int battleship::length = 4;
int battleship::score = 8;

battleship * battleship::clone()
{
	battleship* boat = new battleship();
	copyPositionList(boat->positionList, positionList);

	return boat;
}