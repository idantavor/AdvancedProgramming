#pragma once
#include "Ship.h"

class missileBoat : public Ship
{
	public:
		missileBoat()
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

		missileBoat* clone() override;

		static char symbolAPlayer;
		static char symbolBPlayer;
		static int length;
		static int score;
};
