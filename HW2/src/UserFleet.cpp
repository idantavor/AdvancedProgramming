#include "UserFleet.h"


UserFleet::UserFleet(std::list<Ship*>*shipsL)
{
	shipsList = *shipsL;
}

UserFleet::~UserFleet()
{
	//for (std::list<Ship*>::iterator it = shipsList.begin(); it != shipsList.end(); ++it) {
	//	//delete *it;
	//	//shipsList.erase(it);
	//	int x = 1;
	//}
	//shipsList.clear();
	for (auto &it : shipsList) delete it; 
	shipsList.clear();
}

std::pair<AttackResult, Ship*> UserFleet::executeAttack(Position & p) 
{
	for (std::list<Ship*>::iterator it = shipsList.begin(); it != shipsList.end(); ++it)
	{
		if ((*it)->attackShip(p))
		{
			if((*it)->isShipAlive())
			{
				return std::pair<AttackResult, Ship*>(AttackResult::Hit, *it);
			}
			return std::pair<AttackResult,Ship*> (AttackResult::Sink, *it);
		}
	}

	return std::pair<AttackResult, Ship*>(AttackResult::Miss, nullptr);
}

bool UserFleet::isNotLose()
{
	for (std::list<Ship*>::iterator it = shipsList.begin(); it != shipsList.end(); ++it)
	{
		if ((*it)->isShipAlive())
		{
			return true;
		}
	}

	return false;
}

int UserFleet::getNumberOfShips() const
{
	return shipsList.size();
}

