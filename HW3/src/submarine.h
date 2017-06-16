#pragma once
#include "Ship.h"

class submarine :public Ship
{
	public:
		submarine()
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

		submarine* clone() override;


		static char symbolAPlayer;
		static char symbolBPlayer;
		static int length;
		static int score;
};
