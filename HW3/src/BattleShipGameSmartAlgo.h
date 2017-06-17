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

#define CORD_NO_MATCH Coordinate(-1,-1,-1)


class BattleShipGameSmartAlgo : public IBattleshipGameAlgo {
public:
	BattleShipGameSmartAlgo();
	~BattleShipGameSmartAlgo();

	void setPlayer(int player);
	//here i assume receiving only my player's boats....
	void setBoard(const BoardData& board);		// called once to notify player on his board, 

	bool init(const std::string& path);
	Coordinate attack(); // ask player for his move retruns <row,col>, <-1,-1> if no valid attack is found
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result); // last move result

private:
	//members
	static const Coordinate deltaCords[3];
	int m_rowNum;
	int m_colNum;
	int m_depthNum;
	int m_player;
	int m_mode;	// search or destroy
	char*** m_statusBoard; //the game's state in the eyes of the algo
	int***	m_probBoard; //possible ships in this square
	list<set<Coordinate>> m_shipsUnderAttack;
	list<int> m_leftShipsOfOpponent;
	set<Coordinate> m_myShips;

	Coordinate getNextDestroyPosition();
	void addAttackedPointToShipsList(Coordinate& attackedPoint);//addes the attacked point to the list of ships, either to an existing ship, or creates a new one
	void removeSunkShipFromShipsListAndMarkPlacesAsSunk(Coordinate& attackedPoint);
	Coordinate getSearchPoint();
	bool checkSurrondingPoint(Coordinate& c);//makes sure that a point is eithr out of range, or does not contain a ship, or is part of the attacked ship 
	bool isInBoard(Coordinate& c);
	std::pair<int, int> getMinMax(set<Coordinate>& setOfPoints, char rowOrColOrDepth); //retruns the <min,max> row/col/depth in the set. 'r' for row, 'c' for col 'd' for depth
	set <Coordinate>* getShipUnderAttack(); // returns the first ship under attack, or NULL if such one does not exist
	void tryToPlaceShip(int shipSize); // tries to place a ship in the given stating position
	bool validatePoint(Coordinate& c);  //checks if this position can contain a ship, only relevant during search mode,
	void zeroProbBoard();

	//static functions
	static bool areAdjacent(const Coordinate& p1, const Coordinate& p2);//returns true if the points are next to each other, false o.w
	static bool areShipsAdjacent(const set<Coordinate>& s1, const set<Coordinate>& s2);
	static Coordinate fromRepresntToRealIndex(const Coordinate& c);
	static Coordinate fromRealIndexToRepresnt(const Coordinate& c);
	static void countShips(const BoardData& bd, list<int>& listToFill);
	static void addToList(int item, int times, list<int>& l);
	template <class T>
	static void free3dArray(T*** arr, int d1, int d2 );
};

