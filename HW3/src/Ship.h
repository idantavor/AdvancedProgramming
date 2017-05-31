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
	virtual bool addPointToTheList(int x, int y);
	virtual int getLength()=0;
	virtual int getScore() = 0;
	virtual char getSymbolAPlayer() = 0;
	virtual char getSymbolBPlayer() = 0;
	virtual bool checkShape();
	std::list<Position> positionList;


protected:
	bool isHorizontal;
	int currentLength;


};
