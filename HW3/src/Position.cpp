#include "Position.h"

Position::Position()
{
	x_coord = 0;
	y_coord = 0;
	z_coord = 0;
	isAlive = true;
}

Position::Position(int x, int y, int z) {
	x_coord = x;
	y_coord = y;
	z_coord = z;
	isAlive = true;
}

bool operator==(const Position p1, const Position p2)
{
	if (p1.x_coord == p2.x_coord && p1.y_coord == p2.y_coord && p1.z_coord == p2.z_coord)
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

int Position::getZ() const
{
	return z_coord;
}

void Position::setX(int x)
{
	x_coord = x;
}

void Position::setY(int y)
{
	y_coord = y;
}

void Position::setZ(int z)
{
	z_coord = z;
}

void Position::setPosition(int x, int y)
{
	setX(x);
	setY(y);
}

Position::Position(Position & p)
:x_coord(p.x_coord),y_coord(p.y_coord),z_coord(p.z_coord){
	
}
Position::Position(const Position& p)
: x_coord(p.x_coord), y_coord(p.y_coord), z_coord(p.z_coord){

}

