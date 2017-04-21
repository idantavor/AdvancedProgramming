#pragma once
#include "Ship.h"

class battleship : public Ship
{

	public:
		battleship(std::list<Position>* positionList)
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

char battleship::symbolAPlayer = 'D';
char battleship::symbolBPlayer = 'd';
int battleship::length = 4;
int battleship::score = 8;