#pragma once
#include "ship.h"
#include "IBattleshipGameAlgo.h"

class UserFleet
{
	public:
	explicit UserFleet(std::list<Ship*>*);
	~UserFleet();
	std::pair<AttackResult, Ship*> UserFleet::executeAttack(Position & p) ;
		bool isNotLose();
		int getNumberOfShips() const;
	protected:
		std::list<Ship*> shipsList;
		
};
