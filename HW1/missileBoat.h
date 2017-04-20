#pragma once
#include "Ship.h"

class missileBoat : public Ship
{
	public:
		missileBoat(std::list<Position>* positionList)
			: Ship(length, positionList) {
		}

		int getLength() override {
			return length;
		}

		int getScore() override {
			return score;
		}
		char getSymbolAPlayer() override {
			return symbolAPlayer;
		}

		char getSymbolBPlayer() override {
			return symbolBPlayer;
		}

		static char symbolAPlayer;
		static char symbolBPlayer;
		static int length;
		static int score;
};

char missileBoat::symbolAPlayer = 'P';
char missileBoat::symbolBPlayer = 'p';
int missileBoat::length = 2;
int missileBoat::score = 3;