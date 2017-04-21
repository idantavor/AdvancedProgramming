#pragma once
#ifndef POSTION_H
#define POSTION_H

class Position {
	int x_coord, y_coord; // coordinates of the point

public:
	Position(int, int);
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
	void setPosition(int x, int y);
	bool isAlive;
	friend bool operator== (const Position p1, const Position p2);
};

#endif