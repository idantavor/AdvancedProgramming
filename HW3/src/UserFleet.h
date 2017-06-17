#pragma once
#include "ship.h"
#include "IBattleshipGameAlgo.h"

class UserFleet
{
	public:
	UserFleet()= default;
	~UserFleet();
	std::pair<AttackResult, Ship*> UserFleet::executeAttack(Position & p) ;
	bool isNotLose();
	size_t getNumberOfShips() const;
	void addShipToList(Ship* ship);
	void clone(UserFleet& fleet);
	std::list<Ship*> shipsList;
		
};
