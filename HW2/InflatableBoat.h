#pragma once
#include "Ship.h"

class InflatableBoat: public Ship
{
	public:
		InflatableBoat(std::list<Position>* positionList) 
			: Ship(length, positionList){
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

char InflatableBoat::symbolAPlayer = 'B';
char InflatableBoat::symbolBPlayer = 'b';
int InflatableBoat::length = 1;
int InflatableBoat::score = 2;

