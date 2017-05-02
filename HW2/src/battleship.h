#pragma once
#include "Ship.h"

class battleship : public Ship
{

	public:
		battleship()
			: Ship(length) {
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
