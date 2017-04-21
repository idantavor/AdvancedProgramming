#pragma once
#include "Ship.h"

class submarine :public Ship
{
	public:
		submarine( std::list<Position>* positionList)
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

char submarine::symbolAPlayer = 'M';
char submarine::symbolBPlayer = 'm';
int submarine::length = 3;
int submarine::score = 7;