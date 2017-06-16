#include "missileBoat.h"

char missileBoat::symbolAPlayer = 'P';
char missileBoat::symbolBPlayer = 'p';
int missileBoat::length = 2;
int missileBoat::score = 3;

missileBoat * missileBoat::clone()
{
	missileBoat* boat = new missileBoat();
	copyPositionList(boat->positionList, positionList);

	return boat;
}