#pragma once
#include "IBattleshipGameAlgo.h"
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>


using namespace std;
#define MIN_ROW 1
#define MIN_COL 1
#define PAIR_NO_MATCH std::pair<int,int>(-1,-1)

class BattleShipGameFromFile: public IBattleshipGameAlgo {
public:
	BattleShipGameFromFile(const string& attackFilePath);
	~BattleShipGameFromFile();
	virtual bool init(const std::string& path) override;
	virtual void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move retruns <row,col>, <-1,-1> if no valid attack is found
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
private :
	std::ifstream m_attackFileIFS;
	std::pair<int, int> parseLine(const string& line); // parses a line, returns <row,col> upon suceess ,<-1,-1> upon failure
	int m_rowCount;
	int m_colCount;
	int m_player;
};