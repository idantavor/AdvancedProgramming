#include "InflatableBoat.h"

char InflatableBoat::symbolAPlayer = 'B';
char InflatableBoat::symbolBPlayer = 'b';
int InflatableBoat::length = 1;
int InflatableBoat::score = 2;

InflatableBoat * InflatableBoat::clone()
{
	InflatableBoat* boat = new InflatableBoat();
	copyPositionList(boat->positionList, positionList);

	return boat;
}
