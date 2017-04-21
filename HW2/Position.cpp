#include "Position.h"

Position::Position(int x, int y) {
	x_coord = x;
	y_coord = y;
	isAlive = true;
}

bool operator==(const Position p1, const Position p2)
{
	if (p1.x_coord == p2.x_coord && p1.y_coord == p2.y_coord)
	{
		return true;
	}

	return false;
}

int Position::getX() const
{
	return x_coord;
}

int Position::getY() const
{
	return y_coord;
}

void Position::setX(int x)
{
	x_coord = x;
}

void Position::setY(int y)
{
	y_coord = y;
}

void Position::setPosition(int x, int y)
{
	setX(x);
	setY(y);
}
