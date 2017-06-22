
#include "Point.h"
#include "Matrix.h"
#include <iostream>

/*
template<typename Groups>
void print(const Groups& all_groups) {
	for (const auto& groupType : all_groups) {
		cout << groupType.first << ":" << endl;
		for (const auto& groupOfType : groupType.second) {
			for (const auto& coord : groupOfType) {
				cout << "{ ";
				for (int i : coord) {
					cout << i << ' ';
				}
				cout << "} ";
			}
			cout << endl;
		}
	}
}
*/

int main() {
	Point<2> point(1, 1);
	cout << point[0] << endl;
	Point<2> point2({ 1,1 });
	cout << point2[0] << endl;

	//Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	//auto all_groups = m.groupValues([](auto i) {return islower(i) ? "L" : "U"; });
	//print(all_groups);
}
