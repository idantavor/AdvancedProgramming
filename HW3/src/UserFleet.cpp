#include "UserFleet.h"

UserFleet::~UserFleet()
{
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

size_t UserFleet::getNumberOfShips() const
{
	return shipsList.size();
}

void UserFleet::addShipToList(Ship* ship)
{
	shipsList.push_back(ship);
}

void UserFleet::clone(UserFleet& fleet)
{
	for (std::list<Ship*>::iterator it = shipsList.begin(); it != shipsList.end();++it)
	{
		auto shipToAdd = (*it)->clone();
		fleet.addShipToList(shipToAdd);
	}
}

