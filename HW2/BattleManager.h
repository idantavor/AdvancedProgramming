#pragma once
#include "Ship.h"
#include "UserFleet.h"
#include "ConsolePrinter.h"
#ifndef BATTLE_MANAGER
#define BATTLE_MANAGER
#include "IBattleshipGameAlgo.h"
#include "BattleShipGameFromFile.h"
#include <string>
#include <vector>
#include <direct.h>
#define BOARD_PATH 0
#define DLL_A_PATH 1
#define DLL_B_PATH 2
#define A_TURN 0
#define B_TURN 1
#define NUMBR_OF_SHIPS 5
#define WRONG_SIZE_A(c) cout << "Wrong size or shape for ship " << c << " for player A\n"
#define WRONG_SIZE_B(c) cout <<"Wrong size or shape for ship "  << c << " for player B\n"
#define TOO_MANY_A "Too many ships for player A\n"
#define TOO_FEW_A "Too few ships for player A\n"
#define TOO_MANY_B "Too many ships for player B\n"
#define TOO_FEW_B "Too few ships for player B\n"
#define ADJACENT "Adjacent Ships on Board\n"

using namespace std;

class BattleManager {
		vector<string> filePaths;
		IBattleshipGameAlgo* playerA=nullptr;
		IBattleshipGameAlgo* playerB=nullptr;
		UserFleet* fleetA=nullptr;
		UserFleet* fleetB=nullptr;
		string* board;
		int boardSize;
		bool quiet;
		static bool isKnownLetter(char c);
		ConsolePrinter cp;

	public:
		~BattleManager() {
			if (playerA != nullptr) delete playerA;
			if (playerB != nullptr) delete playerB;
			if (board != nullptr)delete[] board;
			if (fleetA != nullptr) delete fleetA;
			if (fleetB != nullptr) delete fleetB;
		}
		BattleManager(const int boardSize = 10,bool quiet=false,int delay=ATTACKS_DELAY) {
			this->boardSize = boardSize;
			board = new string[boardSize+1];
			this->quiet = quiet;
			this->cp.set_delay(delay);
		}

	void loadBoard(const string& boardPath) const;
		bool validateBoard();
		void buildShip(int x, int y, char shipChar, bool** visitBoard, list<Ship*>* shipsListA,
			list<Ship*>* shipsListB, list<char>* failedCharA, list<char>* failedCharB) const;
		void shipCollectChars(int x, int y, char shipChar, bool** visitBoard, list<Position>* positionList) const;
	bool validateFilesExistanceAndBoardValidate(const std::string& dirPath = "");
		bool runBattle(const string& dirPath);
	void buildUserBoards(char** boardA, char** boardB) const;
	bool isSpacesAreOK(int, int, char) const;
		bool checkSpacesInPosition(int x, int y, char c) const;
};


void getFileNamesFromDir(std::vector<string> &out, const string &directory);
bool has_suffix(const std::string &str, const std::string &suffix);
bool dirExists(const std::string& dirName_in);
void printFinishMsg(int scoreA, int scoreB, int winner);
#endif

