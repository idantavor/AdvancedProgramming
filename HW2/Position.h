#pragma once
#ifndef POSTION_H
#define POSTION_H

class Position {
	int x_coord, y_coord; // coordinates of the point

public:
	Position(int, int);
	int getX() const;
	int getY() const;
	bool isAlive;
	friend bool operator== (const Position p1, const Position p2);
};

#endif