#pragma once
#include "IBattleshipGameAlgo.h"
#include "Position.h"


using namespace std;
#define MIN_ROW 1
#define MIN_COL 1
#define PAIR_NO_MATCH std::pair<int,int>(-1,-1)

class NaiveAlgo : public IBattleshipGameAlgo {
public:
	NaiveAlgo()=default;
	~NaiveAlgo();
	bool init(const std::string& path) override;
	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	pair<int, int> attack() override; // ask player for his move retruns <row,col>, <-1,-1> if no valid attack is found
	void getNextPosition(Position& position) const;
	bool isPositionOk(Position position) const;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
private:
	int m_rowCount =0;
	int m_colCount = 0;
	int m_player = 0;
	//UserFleet* userFleet;
	Position currentPosition;
	char** m_board;
	bool firstPos = true;

};
