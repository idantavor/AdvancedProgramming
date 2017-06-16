#include "submarine.h"

char submarine::symbolAPlayer = 'M';
char submarine::symbolBPlayer = 'm';
int submarine::length = 3;
int submarine::score = 7;

submarine * submarine::clone()
{
	submarine* boat = new submarine();
	copyPositionList(boat->positionList, positionList);

	return boat;
}