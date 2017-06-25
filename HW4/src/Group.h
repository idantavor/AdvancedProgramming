#pragma once
#ifndef GROUP_H_
#define GROUP_H_
#include "Point.h"
#include <list>

template<int nDimensions = 2 >
class Group
{
private:
	std::list<Point<nDimensions>> positionList;

	// comparison, Points
	bool comparePoints(const Point<nDimensions> first, const Point<nDimensions> second)
	{
		unsigned int i = 0;
		while (i<nDimensions)
		{
			if (first[i]<second[i]) return true;
			else if (first[i] > second[i]) return false;
			++i;
		}
		return true;
	}

	void sortGroup() {
		positionList.sort(comparePoints);
	}

	std::list<Point<nDimensions>> getGruop() {
		return positionList;
	}

	void addPoint(Point<nDimensions> p) {
		positionList.push_back(p);
	}

};

#endif 