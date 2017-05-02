#pragma once
#include "ship.h"
#include "IBattleshipGameAlgo.h"

class UserFleet
{
	public:
	UserFleet();
	~UserFleet();
	std::pair<AttackResult, Ship*> UserFleet::executeAttack(Position & p) ;
		bool isNotLose();
		int getNumberOfShips() const;
		void addShipToList(Ship* ship);
	protected:
		std::list<Ship*> shipsList;
		
};
