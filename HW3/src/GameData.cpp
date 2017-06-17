#include "GameData.h"



bool GameData::loadAndValidateBoard(const string boardPath)
{
	board = make_shared<SharedBoard>(boardPath);
	board->loadBoard();
	if(board == nullptr){
		return false;
	}

	int depthSize = board->getDepthSize();
	int rowsSize = board->getRowSize();
	int colsSize = board->getColSize();

	list<char> failedCharA;
	list<char> failedCharB;

	//Insilize visit board
	bool*** visitBoard = new bool**[depthSize];
	for (int i = 0; i < depthSize; i++) {
		visitBoard[i] = new bool*[rowsSize];
		for (int j = 0; j < rowsSize; j++) {
			visitBoard[i][j] = new bool[colsSize];
			for (int k = 0; k < colsSize; k++) {
				visitBoard[i][j][k] = false;
			}
		}
	}

	// check spaces
	bool adjacentError = false;
	for (int i = 0; i < depthSize; i++) {
		for (int j = 0; j < rowsSize; j++) {
			for (int k = 0; k < colsSize; k++) {
				char c = board->getCharAt(j,k,i);
				if (c == ' ') {
					continue;
				}
				if (!isSpacesAreOK( j, k, i, c)) {
					adjacentError = true;
					break;
				}
			}
		}
	}

	//create boards and do checks
	for (int depth = 0; depth < depthSize; depth++) {
		for (int row = 0; row < rowsSize; row++)
		{
			for (int col = 0; col < colsSize; col++) {
				char shipChar = board->getCharAt(row, col, depth);
				if (visitBoard[depth][row][col]) {
					continue;
				}
				if (shipChar == ' ')
				{
					visitBoard[depth][row][col] = true;
					continue;
				}

				buildShip(row, col, depth, shipChar, visitBoard, failedCharA, failedCharB);
			}
		}
	}

	//delete visitBoard;
	for (int i = 0; i < depthSize; i++) {
		for (int j = 0; j < rowsSize; j++) {
			delete[] visitBoard[i][j];
		}
		delete[] visitBoard[i];
	}
	delete[] visitBoard;

	bool checksPass = true;

	if (!failedCharA.empty() || !failedCharB.empty())
	{
		checksPass = false;

	}
	for (std::list<char>::iterator it = (failedCharA).begin(); it != (failedCharA).end(); ++it)
	{
		WRONG_SIZE_A(*it, boardPath);
	}
	for (std::list<char>::iterator it = (failedCharB).begin(); it != (failedCharB).end(); ++it)
	{
		WRONG_SIZE_B(*it, boardPath);
	}


	size_t numberOfShipsA = fleetA.getNumberOfShips();
	size_t numberOfShipsB = fleetB.getNumberOfShips();
	if (numberOfShipsB < numberOfShipsA){
		MORE_SHIP_A(numberOfShipsA, numberOfShipsB, boardPath);
	}

	else if (numberOfShipsB > numberOfShipsA) {
		MORE_SHIP_B(numberOfShipsA, numberOfShipsB, boardPath);
	}
	else {
		// Check if players as same number of ships
		int aShip[4] = { 0 };
		int bShip[4] = { 0 };
		for (std::list<Ship*>::iterator it = (fleetA.shipsList).begin(); it != (fleetA.shipsList).end(); ++it)
		{
			aShip[(*it)->getLength() - 1] += 1;
		}
		for (std::list<Ship*>::iterator it = (fleetB.shipsList).begin(); it != (fleetB.shipsList).end(); ++it)
		{
			bShip[(*it)->getLength() - 1] += 1;
		}

		for (int i = 0; i < 4; i++) {
			if (aShip[i] != bShip[i]) {
				DIFFERENT_SHIPS(boardPath);
				break;
			}
		}

	}


	if (adjacentError)
	{
		Logger("GameData").Warning(ADJACENT(boardPath));
		checksPass = false;
	}

	return checksPass;
}

void GameData::buildShip(unsigned int x, unsigned int y, unsigned int z, char shipChar, bool*** visitBoard, list<char>& failedCharA, list<char>& failedCharB)
{

	if (shipChar == InflatableBoat::symbolAPlayer)
	{
		InflatableBoat* boat = new InflatableBoat();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharA).begin(), (failedCharA).end(), InflatableBoat::symbolAPlayer) != (failedCharA).end());
			if (!found) {
				failedCharA.push_back(InflatableBoat::symbolAPlayer);
			}
			delete boat;
		}
		else
		{
			fleetA.addShipToList(boat);
		}
	}
	else if (shipChar == InflatableBoat::symbolBPlayer)
	{
		InflatableBoat* boat = new InflatableBoat();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharB).begin(), (failedCharB).end(), InflatableBoat::symbolBPlayer) != (failedCharB).end());
			if (!found) {
				failedCharB.push_back(InflatableBoat::symbolBPlayer);
			}
			delete boat;
		}
		else
		{
			fleetB.addShipToList(boat);
		}
	}
	else if (shipChar == missileBoat::symbolAPlayer)
	{
		missileBoat* boat = new missileBoat();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{

			bool found = (std::find((failedCharA).begin(), (failedCharA).end(), missileBoat::symbolAPlayer) != (failedCharA).end());
			if (!found) {
				failedCharA.push_back(missileBoat::symbolAPlayer);
			}
			delete boat;
		}
		else
		{
			fleetA.addShipToList(boat);
		}
	}
	else if (shipChar == missileBoat::symbolBPlayer)
	{
		missileBoat* boat = new missileBoat();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);

		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharB).begin(), (failedCharB).end(), missileBoat::symbolBPlayer) != (failedCharB).end());
			if (!found) {
				failedCharB.push_back(missileBoat::symbolBPlayer);
			}
			delete boat;
		}
		else {
			fleetB.addShipToList(boat);
		}
	}
	else if (shipChar == submarine::symbolAPlayer)
	{
		submarine* boat = new submarine();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharA).begin(), (failedCharA).end(), submarine::symbolAPlayer) != (failedCharA).end());
			if (!found) {
				failedCharA.push_back(submarine::symbolAPlayer);
			}
			delete boat;
		}
		else {
			fleetA.addShipToList(boat);
		}
	}
	else if (shipChar == submarine::symbolBPlayer)
	{
		submarine* boat = new submarine();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharB).begin(), (failedCharB).end(), submarine::symbolBPlayer) != (failedCharB).end());
			if (!found) {
				failedCharB.push_back(submarine::symbolBPlayer);
			}
			delete boat;
		}
		else {
			fleetB.addShipToList(boat);
		}
	}
	else if (shipChar == battleship::symbolAPlayer)
	{
		battleship* boat = new battleship();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);

		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharA).begin(), (failedCharA).end(), battleship::symbolAPlayer) != (failedCharA).end());
			if (!found) {
				failedCharA.push_back(battleship::symbolAPlayer);
			}
			delete boat;
		}
		else {
			fleetA.addShipToList(boat);
		}
	}
	else if (shipChar == battleship::symbolBPlayer)
	{
		battleship* boat = new battleship();
		shipCollectChars(x, y, z, shipChar, visitBoard, *boat);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((failedCharB).begin(), (failedCharB).end(), battleship::symbolBPlayer) != (failedCharB).end());
			if (!found) {
				failedCharB.push_back(battleship::symbolBPlayer);
			}
			delete boat;
		}
		else {
			fleetB.addShipToList(boat);
		}
	}
}


void GameData::shipCollectChars(unsigned int x, unsigned int y, unsigned int z, char shipChar, bool*** visitBoard, Ship& ship) const {
	visitBoard[z][x][y] = true;
	ship.addPointToTheList(x, y, z);
	unsigned int x_i = x + 1;
	if (x_i >= 0 && x_i < board->getRowSize() && !visitBoard[z][x_i][y] && board->getCharAt(x_i, y, z) == shipChar)
	{
		shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
	}
	unsigned int y_i = y + 1;
	if (y_i >= 0 && y_i < board->getColSize() && !visitBoard[z][x][y_i] && board->getCharAt(x, y_i, z) == shipChar)
	{
		shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
	}
	unsigned int z_i = z + 1;
	if (z_i >= 0 && z_i < board->getDepthSize() && !visitBoard[z_i][x][y] && board->getCharAt(x, y, z_i) == shipChar)
	{
		shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
	}
	x_i = x - 1;
	if (x_i >= 0 && x_i < board->getRowSize() && !visitBoard[z][x_i][y] && board->getCharAt(x_i, y, z) == shipChar)
	{
		shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
	}
	y_i = y - 1;
	if (y_i >= 0 && y_i < board->getColSize() && !visitBoard[z][x][y_i] && board->getCharAt(x, y_i, z) == shipChar)
	{
		shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
	}
	z_i = z - 1;
	if (z_i >= 0 && z_i < board->getDepthSize() && !visitBoard[z_i][x][y] && board->getCharAt(x, y, z_i) == shipChar)
	{
		shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
	}
}

bool GameData::isSpacesAreOK(unsigned int x, unsigned int y, unsigned int z, char c) const
{
	if (!checkSpacesInPosition(x + 1, y, z, c)) { return false; }
	if (!checkSpacesInPosition(x - 1, y, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y + 1, z, c)) { return false; }
	if (!checkSpacesInPosition(x, y - 1, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y, z + 1, c)) { return false; }
	if (!checkSpacesInPosition(x, y, z - 1, c)) { return false; }

	return true;
}

bool GameData::checkSpacesInPosition(int  x, int y, int z, char c) const
{
	if (x > 0 && y > 0 && z > 0 && x < board->getRowSize() && y < board->getColSize() && z <  board->getDepthSize() && board->getCharAt(x, y, z) != c && board->getCharAt(x, y, z) != ' ') {
		return false;
	}
	return true;
}

void GameData::clone(GameData & cloned)
{
	fleetA.clone(cloned.fleetA);
	fleetB.clone(cloned.fleetB);
	cloned.board = board;
}


void GameData::setAlgoA(IBattleshipGameAlgo * algo)
{
	this->playerAlgoA = algo;
}

void GameData::setAlgoB(IBattleshipGameAlgo * algo)
{
	this->playerAlgoB = algo;
}

IBattleshipGameAlgo * GameData::getAlgoA()
{
	return this->playerAlgoA;
}

IBattleshipGameAlgo * GameData::getAlgoB()
{
	return this->playerAlgoB;
}

void GameData::setUserFleetA(UserFleet& fleetA)
{
	this->fleetA = fleetA;
}

void GameData::setUserFleetB(UserFleet & fleetB)
{
	this->fleetB = fleetB;
}

