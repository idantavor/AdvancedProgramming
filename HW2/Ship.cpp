#include "ship.h"

Ship::Ship(int len, std::list<Position>* pl)
{
	currentLength = len;
	isHorizontal = false;
	positionList = pl;
}

bool Ship::isPositionInShip(const Position p)
{

	for (std::list<Position>::iterator it = (*positionList).begin(); 
		it != (*positionList).end(); ++it)
	{
		if ((*it) == p)
		{
			return true;
		}
	}
	
	return false;
}


bool Ship::attackShip(const Position& p)
{
	if (!isShipAlive()) {
		return false;
	}
	for (std::list<Position>::iterator it = (*positionList).begin();
		it != (*positionList).end(); ++it)
	{
		if ((*it) == p)
		{
			//positionList.erase(it);
			if ((*it).isAlive) {
				(*it).isAlive = false;
				currentLength--;
			}
			return true;
		}
	}

	return false;
}

bool Ship::addPointToTheList(int x, int y)
{
	(*positionList).push_back(Position(x, y));
	return false;
}

bool Ship::isShipAlive() const
{
	return currentLength != 0;
}

bool Ship::checkShape()
{
	int countX = 0;
	int countY = 0;
	int x =0;
	int y =0 ;
	bool first = true;
	for (std::list<Position>::iterator it = (*positionList).begin(); it != (*positionList).end(); ++it)
	{
		if (first)
		{
			countX++;
			countY++;
			x = (*it).getX();
			y = (*it).getY();
			first = false;
			continue;
		}
		if ((*it).getX() != x)
		{
			countX++;
			x = (*it).getX();
		}
		if ((*it).getY() != y)
		{
			countY++;
			y = (*it).getY();
		}

	}
	if(countX> 1 && countY > 1){
		return false;

	}
	if(countX > 1)
	{
		if (countX != currentLength)
		{
			return false;
		}

		isHorizontal = true;
	}

	else if (countY > 1)
	{
		if (countY != currentLength)
		{
			return false;
		}
		isHorizontal = false;
	}

	else {
		if (countY != currentLength) {
			return false;
		}
	}

	return true;
}

Ship::~Ship()
{
	delete positionList;
}


