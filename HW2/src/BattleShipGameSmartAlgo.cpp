#include "BattleShipGameSmartAlgo.h"
#include "IBattleshipGameAlgo.h"

//C'tor
BattleShipGameSmartAlgo::BattleShipGameSmartAlgo() {
	//here the attachFilePath should exist
	m_mode = SEARCH_MODE;
	m_shipsUnderAttack.clear();
	srand(time(NULL));
}

//D'tor
BattleShipGameSmartAlgo::~BattleShipGameSmartAlgo() {
	//free the board
	for (int i = 0; i < m_rowNum; i++) {
		delete[] m_board[i];
	}
	delete[] m_board;
}

bool BattleShipGameSmartAlgo::init(const std::string& path) {
	//nothing to do
	return true;
}

void BattleShipGameSmartAlgo::setBoard(int player,const char** board, int numRows, int numCols) {
	//initialize and mark board
	m_rowNum = numRows;
	m_colNum = numCols;
	m_player = player;
	m_board = new char*[m_rowNum];
	for (int i = 0; i < m_rowNum; i++) {
		m_board[i] = new char[m_colNum];
	}
	for (int i = 0; i < m_rowNum; i++) {
		for (int j = 0; j < m_rowNum; j++) {
			if (board[i][j] == EMPTY_PLACE) {
				m_board[i][j] = NOT_BOOMBED;
			}
			else {
				m_board[i][j] = SUNK_SHIP; // my ship is there, mark this place as boombed to not hit it again
				m_myShips.insert(std::pair<int,int> (i, j));
			}
		}
	}
}



std::pair<int, int> BattleShipGameSmartAlgo::attack() {
	if (m_mode == SEARCH_MODE) {
		std::pair<int, int> move = PAIR_NO_MATCH;
		auto positions = getPossibleAttackPositions();
		if (positions->size() > 0) { 
			int index = rand() % positions->size(); //0...size-1
			move = (*positions)[index];
		}
		delete positions;
		return fromRealIndexToRepresnt(move);
	}
	else { // DESTROY_MODE 
		return fromRealIndexToRepresnt(getNextDestroyPosition());
	}	
}

std::vector<std::pair<int, int>>* BattleShipGameSmartAlgo::getPossibleAttackPositions() {
	std::vector<std::pair<int, int>>* positions = new std::vector<std::pair<int, int>>();
	for (int i = 0; i < m_rowNum; i++) {
		for (int j = 0; j < m_rowNum; j++) {
			if (isAttackable(std::pair<int,int>(i,j))){
				positions->push_back(std::pair<int, int>(i, j));
			}
		}
	}
	return positions;
}

void BattleShipGameSmartAlgo::addAttackedPointToShipsList(std::pair<int, int>& attackedPoint)
{
	//go over all the ships
	for (list<set<std::pair<int, int>>>::iterator shipIter = m_shipsUnderAttack.begin(); shipIter != m_shipsUnderAttack.end(); shipIter++) {
		//for each ship go over all points
		for (set<std::pair<int, int>>::iterator pointIter = shipIter->begin(); pointIter != shipIter->end(); pointIter++) {
			if (areAdjacent(attackedPoint, *pointIter)) {
				//the attacked point is part of this ship
				shipIter->insert(attackedPoint);
				return;
			}
		}
	}
	//if we got here meaning we could not find an appropriate place for this point, add it as a new ship
	set<pair<int, int>> newShip;
	newShip.insert(attackedPoint);
	m_shipsUnderAttack.push_back(newShip);
}

void BattleShipGameSmartAlgo::removeSunkShipFromShipsListAndMarkPlacesAsSunk(std::pair<int, int>& attackedPoint)
{
	//take all the points that are part of the sunk ship, remove the resulting ships from m_shipsUnderAttack 
	set<pair<int, int>> sunkShip;
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
	//now we have all the points of the sunk ship, all is left is to mark them
	for (auto iter = sunkShip.begin(); iter != sunkShip.end(); iter++) {
		m_board[iter->first][iter->second] = SUNK_SHIP;
	}
}



std::pair<int, int> BattleShipGameSmartAlgo::getNextDestroyPosition() {
	std::pair<int, int> result;
	set < std::pair<int, int>>* shipUnderAttack = getShipUnderAttack();
	if (shipUnderAttack == nullptr) {//sanity shouldn't happen
		return PAIR_NO_MATCH;
	}
	//first collect possible points
	std::list<std::pair<int, int>>  pointsToCheck;
	if (shipUnderAttack->size() == 1) {//if direction of ship is unknown check all directions
		//get the attacked point
		std::pair<int, int> attackedPoint = *(shipUnderAttack->begin());
		pointsToCheck.push_back(std::pair<int, int>(attackedPoint.first - 1, attackedPoint.second));//up
		pointsToCheck.push_back(std::pair<int, int>(attackedPoint.first , attackedPoint.second + 1));//right
		pointsToCheck.push_back(std::pair<int, int>(attackedPoint.first + 1, attackedPoint.second)); // down
		pointsToCheck.push_back(std::pair<int, int>(attackedPoint.first , attackedPoint.second - 1)); // left
	}
	else if (shipUnderAttack->size() > 1) {// more than one point -> direction is known
		//take two points from the attacked ship
		std::set<std::pair<int, int>>::const_iterator it = shipUnderAttack->cbegin();
		std::pair<int, int> p1 = *it;
		std::pair<int, int> p2 = *(++it);
		if (abs(p1.first - p2.first) > 0) {// row - row > 0 -> vertical
			std::pair<int, int>	rowMinMax = getMinMax(*shipUnderAttack, 'r');
			int col = p1.second;
			pointsToCheck.push_back(std::pair<int, int>(rowMinMax.first - 1,col));//up
			pointsToCheck.push_back(std::pair<int, int>(rowMinMax.second + 1, col)); // down
		}
		else if (abs(p1.second - p2.second) > 0) { // col-col > 0 -> horizontol
			std::pair<int, int>	colMinMax = getMinMax(*shipUnderAttack, 'c');
			int row = p1.first;
			pointsToCheck.push_back(std::pair<int, int>(row, colMinMax.first - 1));//left
			pointsToCheck.push_back(std::pair<int, int>(row, colMinMax.second + 1)); // right
		}
		else {//shouldnt happen
			return PAIR_NO_MATCH;
		}
	}
	else { // shouldn't happen
		return PAIR_NO_MATCH;
	}
	//now remove all invalid points
	for (auto iter = pointsToCheck.begin(); iter != pointsToCheck.end(); ) {
		if (!isAttackable(*iter)) {
			iter = pointsToCheck.erase(iter);
		}
		else {
			iter++;
		}
	}
	//here we are left with 0-4 attackable points, radnomize the requierd one 
	if (pointsToCheck.size()<=0){
		return PAIR_NO_MATCH; //could not find any attackable points to continue destrying the ship, werid!
	}
	int index = rand() % pointsToCheck.size();
	//get to the randomized one, since this is up to 4 steps away it is pratcily O(1)
	for (auto iter = pointsToCheck.begin(); iter != pointsToCheck.end(); iter++) {
		if (index == 0) {
			return (*iter);
		}
		index--;
	}
	//default, shouldn't get here
	return PAIR_NO_MATCH;
}

bool BattleShipGameSmartAlgo::isAttackable(const std::pair<int, int>& rowCol) {
	//make sure it is on the board
	if (!isInBoard(rowCol)) {
		return false;
	}
	//make sure it was not already attacked
	if (m_board[rowCol.first][rowCol.second]!=NOT_BOOMBED) {
		return false;
	}
//now we want to look at all the points around this point
// all the points around this point should not contain a ship
return (
	checkSinglePoint(std::pair<int, int>(rowCol.first - 1, rowCol.second)) &&//up
	checkSinglePoint(std::pair<int, int>(rowCol.first, rowCol.second + 1)) &&// right
	checkSinglePoint(std::pair<int, int>(rowCol.first + 1, rowCol.second)) &&//down
	checkSinglePoint(std::pair<int, int>(rowCol.first, rowCol.second - 1))//left
	);
}


bool BattleShipGameSmartAlgo::checkSinglePoint(const std::pair<int, int>& rowCol) {
	if (!isInBoard(rowCol)) { // if point not in board it does not empose any restrications
		return true;
	}
	if (m_board[rowCol.first][rowCol.second] != SUNK_SHIP) { // if point is not a ship it does not impose any restrications
		return true;
	}
	return false;
}


void BattleShipGameSmartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	std::pair<int, int> realPoint = fromRepresntToRealIndex(row, col);
	row = realPoint.first;
	col = realPoint.second;
	if (m_myShips.find(realPoint) != m_myShips.end()) {//other player hit me :(
		//do nothing 
		return;
	}
	if (result == AttackResult::Hit) {
		m_mode = DESTROY_MODE;
		addAttackedPointToShipsList(realPoint);
		m_board[row][col] = BOOMBED_HIT;
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
		m_board[row][col] = BOOMBED_EMPTY;
	}
}

bool BattleShipGameSmartAlgo::isInBoard(const std::pair<int, int>& rowCol)
{
	int row = rowCol.first;
	int col = rowCol.second;
	if (row >= 0 && row < m_rowNum && col >= 0 && col < m_colNum) {
		return true;
	}
	else {
		return false;
	}
}

std::pair<int, int> BattleShipGameSmartAlgo::getMinMax(set<std::pair<int, int>>& setOfPoints, char rowOrCol)
{
	int min = std::max(m_colNum, m_rowNum);
	int max = 0;
	for (auto iter = setOfPoints.begin(); iter != setOfPoints.end(); iter++) {
		int value = -1;
		if (rowOrCol == 'r') {
			value = (*iter).first;
		}
		else { //rowOrCol=='c'
			value = (*iter).second;
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

std::pair<int, int> BattleShipGameSmartAlgo::fromRepresntToRealIndex(int row, int col)
{
	return std::pair<int, int>(row - 1, col - 1);
}

std::pair<int, int> BattleShipGameSmartAlgo::fromRealIndexToRepresnt(std::pair<int, int> rowCol)
{
	if (rowCol == PAIR_NO_MATCH) {
		return PAIR_NO_MATCH;
	}
	else {
		return std::pair<int, int>(rowCol.first + 1, rowCol.second + 1);
	}
}

set<std::pair<int, int>>* BattleShipGameSmartAlgo::getShipUnderAttack()
{
	if (m_shipsUnderAttack.size() == 0 ){
		return nullptr;
	}
	return &(*m_shipsUnderAttack.begin());
}

bool BattleShipGameSmartAlgo::areAdjacent(const std::pair<int, int>& p1, const std::pair<int, int>& p2)
{
	int distance = abs(p1.first - p2.first) + abs(p1.second - p2.second);
	if (distance == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BattleShipGameSmartAlgo::areShipsAdjacent(const set<std::pair<int, int>>& s1, const set<std::pair<int, int>>& s2)
{
	for (set<std::pair<int, int>>::iterator p1Iter = s1.begin(); p1Iter != s1.end(); p1Iter++) {
		for (set<std::pair<int, int>>::iterator p2Iter = s2.begin(); p2Iter != s2.end(); p2Iter++) {
			if (areAdjacent(*p1Iter, *p2Iter)) {
				return true;
			}
		}
	}
	return false;
}

IBattleshipGameAlgo* GetAlgorithm() {
	return new BattleShipGameSmartAlgo();
};

