#pragma once
#ifndef GROUP_VALUES_H_
#define GROUP_VALUES_H_
#include "GroupType.h"
#include "Group.h"
#include <list>
#include <string>

using namespace std;


template<int nDimensions = 2 >
class GroupValues
{
private:
	std::list<GroupType<nDimensions>> values;

public

	void addGroupType(GroupType<nDimensions> g) {
		this->values.push_back(g);
	}

	void addGroupType(string type) {

		this->values.push_back(new GroupType(type));
	}

};


#endif 