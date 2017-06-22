#pragma once
#ifndef GROUP_TYPE_H_
#define GROUP_TYPE_H_
#include "Group.h"
#include <list>
#include <string>

using namespace std;


template<int nDimensions = 2 >
class GroupType
{
private:
	string first;
	std::list<Group<nDimensions>> secend;

public 
	GroupType(string type) {
		this->first = type;
	}

	
	void setFirst(string type) {
		this->first = type;
	}

	void addGroup(Group<nDimensions> g) {
		this->secend.push_back(g.sortGroup());
	}
};


#endif 