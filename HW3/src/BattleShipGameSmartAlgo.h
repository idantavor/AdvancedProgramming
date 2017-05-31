#pragma once
#include "IBattleshipGameAlgo.h"
#include "Position.h"
#include <set>
#include <vector>
#include <list>
#include <cmath> 
#include <algorithm>  
#include <time.h>

//GENERAL NOTICE
//all public functions (which are part of the "interface") reccieve and send postions starting from 1
//all private functions "talk" in real indexes - staring from 0 ...

using namespace std;
//algo has two mode , search and destroy
#define SEARCH_MODE 0
#define DESTROY_MODE 1

//on board,
#define BOOMBED_EMPTY 'B' // boomeb
#define NOT_BOOMBED 'N'
#define SUNK_SHIP 'S' // after ship is sunk
#define BOOMBED_HIT 'H' //ship is hit but not sunk yet

#define EMPTY_PLACE ' '

#define PAIR_NO_MATCH std::pair<int,int>(-1,-1)

enum class AttackDirection {
	UP, DOWN, RIGHT,LEFT
};

class BattleShipGameSmartAlgo : public IBattleshipGameAlgo {
public:
	BattleShipGameSmartAlgo();
	~BattleShipGameSmartAlgo();

	//here i assume receiving only my player's boats....
	void setBoard(int player, const char** board, int numRows, int numCols);		// called once to notify player on his board, 

	bool init(const std::string& path);
	std::pair<int, int> attack(); // ask player for his move retruns <row,col>, <-1,-1> if no valid attack is found
	void notifyOnAttackResult(int player, int row, int col, AttackResult result); // notify on last move result

private:
	int m_rowNum;
	int m_colNum;
	int m_player;
	int m_mode; // search or destroy
	char** m_board; //the board the algo uses

	list<set<std::pair<int, int>>> m_shipsUnderAttack;
	set<std::pair<int, int>> m_myShips;
	AttackDirection m_direction;

	std::pair<int, int> getNextDestroyPosition();

	std::vector<std::pair<int, int>>* getPossibleAttackPositions();

	void addAttackedPointToShipsList(std::pair<int, int>& attackedPoint);//addes the attacked point to the list of ships, either to an existing ship, or creates a new one
	void removeSunkShipFromShipsListAndMarkPlacesAsSunk(std::pair<int, int>& attackedPoint);

	bool isAttackable(const std::pair<int, int>& rowCol);

	bool checkSinglePoint(const std::pair<int, int>& rowCol);//makes sure that a point is eithr out of range, or does not contain a ship, or is part of the attacked ship 

	bool isInBoard(const std::pair<int, int>& rowCol);
	std::pair<int, int> getMinMax(set<std::pair<int, int>>& setOfPoints, char rowOrCol); //retruns the <min,max> row/col in the set. 'r' for row, 'c' for col
	std::pair<int, int> fromRepresntToRealIndex(int row, int col);
	std::pair<int, int> fromRealIndexToRepresnt(std::pair<int, int> rowCol);//converting from real indexes s.a 0,0 to Represenable indexes 1,1
	set < std::pair<int, int>>* getShipUnderAttack(); // returns the first ship under attack, or NULL if such one does not exist
	bool areAdjacent(const std::pair<int, int>& p1, const std::pair<int, int>& p2);//returns true if the points are next to each other, false o.w
	bool areShipsAdjacent(const set<std::pair<int, int>>& p1, const set<std::pair<int, int>>& p2);
};