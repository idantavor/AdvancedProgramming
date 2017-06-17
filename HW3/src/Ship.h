#pragma once
#include "Position.h"
#include <list>

class Ship {
public:
	virtual ~Ship() = default;
	explicit Ship(int len);
	virtual bool isPositionInShip(const Position p);
	virtual bool isShipAlive() const;
	virtual bool attackShip(const Position& p);
	virtual bool addPointToTheList(int x, int y, int z);
	virtual int getLength()=0;
	virtual int getScore() = 0;
	virtual char getSymbolAPlayer() = 0;
	virtual char getSymbolBPlayer() = 0;
	virtual bool checkShape();
	void Ship::copyPositionList(std::list<Position> &newPositionList, std::list<Position> &oldPositionList);
	std::list<Position> positionList;
	virtual Ship* clone() = 0;


protected:
	int currentLength;


};
