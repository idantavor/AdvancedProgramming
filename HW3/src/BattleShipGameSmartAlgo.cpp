#include "BattleShipGameSmartAlgo.h"
#include "IBattleshipGameAlgo.h"
#include "InflatableBoat.h"
#include "missileBoat.h"
#include "submarine.h"
#include "battleship.h"
#include "Utility.h"
#include <iostream>

const Coordinate BattleShipGameSmartAlgo::deltaCords[3] = {Coordinate(1,0,0), Coordinate(0,1,0),Coordinate(0,0,1) };

//C'tor
BattleShipGameSmartAlgo::BattleShipGameSmartAlgo() {
	//here the attachFilePath should exist
	m_mode = SEARCH_MODE;
	m_shipsUnderAttack.clear();
	srand(static_cast<unsigned int>(time(nullptr)));
}

//D'tor
BattleShipGameSmartAlgo::~BattleShipGameSmartAlgo() {
	/*free3dArray(m_statusBoard, m_rowNum, m_colNum);
	free3dArray(m_probBoard, m_rowNum, m_colNum);*/
}

void BattleShipGameSmartAlgo::setPlayer(int player)
{
	m_player = player;
}

bool BattleShipGameSmartAlgo::init(const std::string& path) {
	//nothing to do
	return true;
}

void BattleShipGameSmartAlgo::setBoard(const BoardData& board) {
	//initialize and mark board
	m_rowNum = board.rows();
	m_colNum = board.cols();
	m_depthNum = board.depth();
	//init status board
	m_statusBoard = new char**[m_rowNum];
	for (int i = 0; i < m_rowNum; i++) {
		m_statusBoard[i] = new char*[m_colNum];
		for (int j = 0; j < m_colNum; j++) {
			m_statusBoard[i][j] = new char[m_depthNum];
		}
	}
	for (int x = 0; x < m_rowNum; x++) {
		for (int y = 0; y < m_colNum; y++) {
			for (int z = 0; z < m_depthNum; z++) {
				if (board.charAt(fromRealIndexToRepresnt(Coordinate(x,y,z))) == EMPTY_PLACE) {
					m_statusBoard[x][y][z] = NOT_BOOMBED;
				}
				else {
					m_statusBoard[x][y][z] = SUNK_SHIP; // my ship is there, mark this place as boombed to not hit it again
					m_myShips.insert(Coordinate(x, y, z));
				}
			}
		}
	}
	//alocate prob board 
	m_probBoard = new int**[m_rowNum];
	for (int i = 0; i < m_rowNum; i++) {
		m_probBoard[i] = new int*[m_colNum];
		for (int j = 0; j < m_colNum; j++) {
			m_probBoard[i][j] = new int[m_depthNum];
		}
	}
	//init my oponenets ships list
	countShips(board,m_leftShipsOfOpponent);
}



Coordinate BattleShipGameSmartAlgo::attack() {
	if (m_mode == SEARCH_MODE) {
		return fromRealIndexToRepresnt(getSearchPoint());
	}
	else { // DESTROY_MODE 
		return fromRealIndexToRepresnt(getNextDestroyPosition());
	}	
}


void BattleShipGameSmartAlgo::addAttackedPointToShipsList(Coordinate& attackedPoint)
{
	//go over all the ships
	for (list<set<Coordinate>>::iterator shipIter = m_shipsUnderAttack.begin(); shipIter != m_shipsUnderAttack.end(); shipIter++) {
		//for each ship go over all points
		for (set<Coordinate>::iterator pointIter = shipIter->begin(); pointIter != shipIter->end(); pointIter++) {
			if (areAdjacent(attackedPoint, *pointIter)) {
				//the attacked point is part of this ship
				shipIter->insert(attackedPoint);
				return;
			}
		}
	}
	//if we got here meaning we could not find an appropriate place for this point, add it as a new ship
	set<Coordinate> newShip;
	newShip.insert(attackedPoint);
	m_shipsUnderAttack.push_back(newShip);
}

void BattleShipGameSmartAlgo::removeSunkShipFromShipsListAndMarkPlacesAsSunk(Coordinate& attackedPoint)
{
	//take all the points that are part of the sunk ship, remove the resulting ships from m_shipsUnderAttack 
	set<Coordinate> sunkShip;
	sunkShip.insert(attackedPoint);
	bool foundShip = true;
	while (foundShip) {
		foundShip = false;
		//go over all the ships
		for (auto shipIter = m_shipsUnderAttack.begin(); shipIter != m_shipsUnderAttack.end(); ) {
			if (areShipsAdjacent(sunkShip, *shipIter)) {
				//this ship is part of the sunk ship
				sunkShip.insert(shipIter->begin(), shipIter->end()); //unite them
				shipIter = m_shipsUnderAttack.erase(shipIter); // erase the ship
				foundShip = true;//while should continue
			}
			else {
				shipIter++;
			}
		}
	}
	//now we have all the points of the sunk ship, all is left is to mark them and cnt the size
	int size = 0;
	for (auto iter = sunkShip.begin(); iter != sunkShip.end(); iter++) {
		m_statusBoard[iter->row][iter->col][iter->depth] = SUNK_SHIP;
		size++;
	}
	//we also want to remove the relevant legnth from list of oponnet ships
	for (auto iter = m_leftShipsOfOpponent.begin(); iter != m_leftShipsOfOpponent.end(); iter++) {
		if (size == (*iter)) {
			m_leftShipsOfOpponent.erase(iter);
			break;
		}
	}
}



Coordinate& BattleShipGameSmartAlgo::getNextDestroyPosition() {
	std::pair<int, int> result;
	set<Coordinate>* shipUnderAttack = getShipUnderAttack();
	if (shipUnderAttack == nullptr) {//sanity shouldn't happen
		return CORD_NO_MATCH;
	}
	//first collect possible points
	std::list<Coordinate>  pointsToCheck;
	if (shipUnderAttack->size() == 1) {//if direction of ship is unknown check all directions
		//get the attacked point
		Coordinate attackedPoint = *(shipUnderAttack->begin());
		pointsToCheck.push_back(Coordinate(attackedPoint.row - 1, attackedPoint.col, attackedPoint.depth));//up
		pointsToCheck.push_back(Coordinate(attackedPoint.row + 1, attackedPoint.col, attackedPoint.depth)); // down
		pointsToCheck.push_back(Coordinate(attackedPoint.row ,attackedPoint.col + 1, attackedPoint.depth));//right
		pointsToCheck.push_back(Coordinate(attackedPoint.row, attackedPoint.col - 1, attackedPoint.depth));//left
		pointsToCheck.push_back(Coordinate(attackedPoint.row, attackedPoint.col, attackedPoint.depth + 1));//in
		pointsToCheck.push_back(Coordinate(attackedPoint.row, attackedPoint.col, attackedPoint.depth - 1));//out
	}
	else if (shipUnderAttack->size() > 1) {// more than one point -> direction is known
		//take two points from the attacked ship
		std::set<Coordinate>::const_iterator it = shipUnderAttack->cbegin();
		Coordinate p1 = *it;
		Coordinate p2 = *(++it);
		if (abs(p1.row - p2.row) > 0) {// row - row > 0 -> vertical
			std::pair<int, int>	rowMinMax = getMinMax(*shipUnderAttack, 'r');
			pointsToCheck.push_back(Coordinate(rowMinMax.first - 1, p1.col, p1.depth));//up
			pointsToCheck.push_back(Coordinate(rowMinMax.first + 1, p1.col, p1.depth)); // down
		}
		else if (abs(p1.col - p2.col) > 0) { // col-col > 0 -> horizontol
			std::pair<int, int>	colMinMax = getMinMax(*shipUnderAttack, 'c');
			pointsToCheck.push_back(Coordinate(p1.row, colMinMax.first - 1,p1.depth));//left
			pointsToCheck.push_back(Coordinate(p1.row, colMinMax.second + 1,p1.depth)); // right
		}
		else if (abs(p1.depth - p2.depth) > 0) { // depth-depth > 0 -> depthitcal
			std::pair<int, int>	depthMinMax = getMinMax(*shipUnderAttack, 'd');
			pointsToCheck.push_back(Coordinate(p1.row, p1.col, depthMinMax.first - 1));//out
			pointsToCheck.push_back(Coordinate(p1.row, p1.col, depthMinMax.second + 1)); // right
		}
		else {//shouldnt happen
			return CORD_NO_MATCH;
		}
	}
	else { // less than one point on attacked hsip
		return CORD_NO_MATCH;
	}
	//now remove all invalid points
	for (auto iter = pointsToCheck.begin(); iter != pointsToCheck.end(); ) {
		if (!validatePoint(*iter)) {
			iter = pointsToCheck.erase(iter);
		}
		else {
			iter++;
		}
	}
	//here we are left with 0-6 attackable points, radnomize the requierd one 
	if (pointsToCheck.size()<=0){
		return CORD_NO_MATCH; //could not find any attackable points to continue destrying the ship, weird!
	}
	int index = rand() % pointsToCheck.size();
	//get to the randomized one, since this is up to 6 steps away it is pratcily O(1)
	for (auto iter = pointsToCheck.begin(); iter != pointsToCheck.end(); iter++) {
		if (index == 0) {
			return (*iter);
		}
		index--;
	}
	//default, shouldn't get here
	return CORD_NO_MATCH;
}



bool BattleShipGameSmartAlgo::checkSurrondingPoint(Coordinate& c) {
	if (!isInBoard(c)) { // if point not in board it does not empose any restrications
		return true;
	}
	if (m_statusBoard[c.row][c.col][c.depth] != SUNK_SHIP) { // if point is not a ship it does not impose any restrications
		return true;
	}
	return false;
}

bool BattleShipGameSmartAlgo::validatePoint(Coordinate& c) {
	//make sure it is on the board
	if (!isInBoard(c)) {
		return false;
	}
	//make sure it was not already attacked
	if (m_statusBoard[c.row][c.col][c.depth] != NOT_BOOMBED) {
		return false;
	}
	//make sure it's surronding points do not impose a restriction 
	return (
		checkSurrondingPoint(Coordinate(c.row - 1, c.col, c.depth)) &&//up
		checkSurrondingPoint(Coordinate(c.row + 1, c.col, c.depth)) &&//down
		checkSurrondingPoint(Coordinate(c.row , c.col - 1, c.depth)) &&//left
		checkSurrondingPoint(Coordinate(c.row , c.col + 1, c.depth)) &&// right
		checkSurrondingPoint(Coordinate(c.row, c.col, c.depth - 1)) &&// in
		checkSurrondingPoint(Coordinate(c.row, c.col, c.depth +1))// out
		);
}

void BattleShipGameSmartAlgo::tryToPlaceShip(int shipSize) {
	//go over all starting points
	for (int r = 0; r < m_rowNum; r++) {
		for (int c = 0; c < m_colNum; c++) {
			for (int d = 0; d < m_depthNum; d++) {
				//for each starting point, go in all directions 
				for (int direction = 0; direction < 3; direction++) {
					bool shipFits = true;
					//for each direction make sure all points fit for this ship
					for (int i = 0; i < shipSize; i++) {
						if (!validatePoint(Coordinate(r + (i *deltaCords[direction].row ),
													  c + (i *deltaCords[direction].col ),
													  d + (i *deltaCords[direction].depth ))))
						{
							shipFits = false;
							break;
						}
					}
					//if all points fits for this ship -> ship fits -> update the probability matrix
					if (shipFits) {
						for (int i = 0; i < shipSize; i++) {
							int row = r + (i *deltaCords[direction].row);
							int col = c + (i *deltaCords[direction].col);
							int depth = d + (i *deltaCords[direction].depth);
							m_probBoard[r][c][d] += 1;
						}
					}
				}
			}
		}
	}
}

Coordinate& BattleShipGameSmartAlgo::getSearchPoint() {
	//first zero the prob matrix
	zeroProbBoard();
	//refill it according to current status
	for (list<int>::iterator iter = m_leftShipsOfOpponent.begin(); iter != m_leftShipsOfOpponent.end(); iter++) {
		tryToPlaceShip(*iter);
	}
	//find most propable Coorednate
	int maxCnt = 0;
	Coordinate maxCord = CORD_NO_MATCH;
	for (int x; x < m_rowNum; x++) {
		for (int y; y < m_colNum; y++) {
			for (int z; z < m_depthNum; z++) {
				if (m_probBoard[x][y][z] > maxCnt) {
					maxCnt = m_probBoard[x][y][z];
					maxCord = Coordinate(x, y, z);
				}
			}
		}
	}
	return maxCord;
}


void BattleShipGameSmartAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result){
	Coordinate realPoint = fromRepresntToRealIndex(move);
	if (realPoint == CORD_NO_MATCH) {
		return; // invalid attack position, nothing to do with it
	}
	if (m_myShips.find(realPoint) != m_myShips.end()) {//other player hit me :(
		//do nothing 
		return;
	}
	if (result == AttackResult::Hit) {
		m_mode = DESTROY_MODE;
		addAttackedPointToShipsList(realPoint);
		m_statusBoard[realPoint.row][realPoint.col][realPoint.depth] = BOOMBED_HIT;
	}
	else if (result == AttackResult::Sink) {
		removeSunkShipFromShipsListAndMarkPlacesAsSunk(realPoint);
		if (m_shipsUnderAttack.size() > 0) {
			m_mode = DESTROY_MODE;
		}
		else {
			m_mode = SEARCH_MODE;
		}
	}
	else {
		m_statusBoard[realPoint.row][realPoint.col][realPoint.depth] = BOOMBED_EMPTY;
	}
}

bool BattleShipGameSmartAlgo::isInBoard(Coordinate& c)
{
	if (c.row >= 0 && c.row < m_rowNum &&
		c.col >= 0 && c.col < m_colNum && 
		c.depth >= 0 && c.depth < m_depthNum) {
		return true;
	}
	else {
		return false;
	}
}

std::pair<int, int> BattleShipGameSmartAlgo::getMinMax(set<Coordinate>& setOfPoints, char rowOrColOrDepth)
{
	int min = std::max(m_colNum, m_rowNum, m_depthNum);
	int max = 0;
	for (auto iter = setOfPoints.begin(); iter != setOfPoints.end(); iter++) {
		int value = -1;
		if (rowOrColOrDepth == 'r') {
			value = (*iter).row;
		}
		else if (rowOrColOrDepth == 'c') {
			value = (*iter).col;
		}
		else { //rowOrColOrDepth = d
			value = (*iter).depth;
		}
		if (value > max) {
			max = value;
		}
		if (value < min) {
			min = value;
		}
	}
	return std::pair<int, int>(min, max);
}

Coordinate& BattleShipGameSmartAlgo::fromRepresntToRealIndex(const Coordinate& c)
{
	if (c == CORD_NO_MATCH) {
		return CORD_NO_MATCH;
	}
	else {
		return Coordinate(c.row - 1, c.col - 1, c.depth - 1);
	}
}

Coordinate& BattleShipGameSmartAlgo::fromRealIndexToRepresnt(const Coordinate& c)
{
	if (c == CORD_NO_MATCH) {
		return CORD_NO_MATCH;
	}
	else {
		return Coordinate(c.row + 1, c.col + 1, c.depth + 1);
	}
}


set<Coordinate>* BattleShipGameSmartAlgo::getShipUnderAttack()
{
	if (m_shipsUnderAttack.size() == 0 ){
		return nullptr;
	}
	return &(*m_shipsUnderAttack.begin());
}

bool BattleShipGameSmartAlgo::areAdjacent(const Coordinate& p1, const Coordinate& p2)
{
	int distance = abs(p1.row - p2.row) + abs(p1.col - p2.col) + abs(p1.depth - p2.depth);
	if (distance == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BattleShipGameSmartAlgo::areShipsAdjacent(const set<Coordinate>& s1, const set<Coordinate>& s2)
{
	for (set<Coordinate>::iterator p1Iter = s1.begin(); p1Iter != s1.end(); p1Iter++) {
		for (set<Coordinate>::iterator p2Iter = s2.begin(); p2Iter != s2.end(); p2Iter++) {
			if (areAdjacent(*p1Iter, *p2Iter)) {
				return true;
			}
		}
	}
	return false;
}

void BattleShipGameSmartAlgo::zeroProbBoard() 
{
	for (int x; x < m_rowNum; x++) {
		for (int y; y < m_colNum; y++) {
			for (int z; z < m_depthNum; z++) {
				m_probBoard[x][y][z] = 0;
			}
		}
	}
}
void BattleShipGameSmartAlgo::countShips(const BoardData& bd, list<int>& listToFill) {
	int cntInflt, cntMisl, cntSub, cntBtl = 0;
	for (int x = 0; x < bd.rows() ; x++) {
		for (int y = 0; y <  bd.cols(); y++) {
			for (int z = 0; z < bd.depth(); z++) {
				char c = bd.charAt(fromRealIndexToRepresnt(Coordinate(x, y, z)));
				switch (tolower(c))
				{
				case 'm': // submarine
					cntSub += 1;
					break;

				case 'p': // missle boat
					cntMisl += 1;
					break;
					  
				case 'd': // battle ship 
					cntBtl += 1;
					break;

				case 'b': // inflateable boat
					cntInflt += 1;
					break;
				default:
					break;
				}
			}
		}
	}
	addToList(submarine::length, cntSub / submarine::length, listToFill);
	addToList(missileBoat::length, cntMisl / missileBoat::length, listToFill);
	addToList(battleship::length, cntBtl / battleship::length, listToFill);
	addToList(InflatableBoat::length, cntInflt / InflatableBoat::length, listToFill);
}


void BattleShipGameSmartAlgo::addToList(int item, int times, list<int>& l) {
	for (int i = 0; i < times; i++) {
		l.push_back(item);
	}
}

//template<T>
//void BattleShipGameSmartAlgo::free3dArray(T*** arr, int d1, int d2)
//{
//	for (int i = 0; i < d1; i++) {
//		for (int j = 0; j < d2; j++) {
//			delete[] arr[i][j];
//		}
//		delete arr[i];
//	}
//	delete[] arr;
//}

IBattleshipGameAlgo* GetAlgorithm() {
	return new BattleShipGameSmartAlgo();
};

