#include "GameData.h"



bool GameData::loadAndValidateBoard(const string boardPath)
{
	BoardParser parser(boardPath);
	board = parser.loadBoard();
	if(board == nullptr){
		return false;
	}

	depthSize = parser.getDepthSize();
	rowsSize = parser.getRowSize();
	colsSize = parser.getColSize();

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
				char c = board[i][j][k];
				if (c == ' ') {
					continue;
				}
				if (!isSpacesAreOK(i, j, k, c)) {
					adjacentError = true;
					break;
				}
			}
		}
	}

	//create boards and do checks
	for (int depth = 0; depth < depthSize; depth++) {
		string* boardDepthString = board[depth];
		for (int row = 0; row < rowsSize; row++)
		{
			string boardRowLength = boardDepthString[row];
			for (int col = 0; col < colsSize; col++) {
				char shipChar = boardRowLength[col];
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
		WRONG_SIZE_A(*it);
	}
	for (std::list<char>::iterator it = (failedCharB).begin(); it != (failedCharB).end(); ++it)
	{
		WRONG_SIZE_B(*it);
	}

	if (adjacentError)
	{
		cout << ADJACENT;
		checksPass = false;
	}

	for (int i = 0; i < depthSize; i++) {
		cout << "new" << endl;
		cout  << endl;
		for (int j = 0; j < rowsSize; j++) {
			cout << board[i][j] << endl;
		}
	}

	return checksPass;
}

void GameData::buildShip( int x, int y, int z, char shipChar, bool*** visitBoard, list<char>& failedCharA, list<char>& failedCharB)
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


void GameData::shipCollectChars(int x, int y, int z, char shipChar, bool*** visitBoard, Ship& ship) const {
	visitBoard[z][x][y] = true;
	ship.addPointToTheList(x, y, z);
	int x_i = x + 1;
	if (x_i >= 0 && x_i < rowsSize && !visitBoard[z][x_i][y] && board[z][x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
	}
	int y_i = y + 1;
	if (y_i >= 0 && y_i < colsSize && !visitBoard[z][x][y_i] && board[z][x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
	}
	int z_i = z + 1;
	if (z_i >= 0 && z_i < depthSize && !visitBoard[z_i][x][y] && board[z_i][x][y] == shipChar)
	{
		shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
	}
	x_i = x - 1;
	if (x_i >= 0 && x_i < rowsSize && !visitBoard[z][x_i][y] && board[z][x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
	}
	y_i = y - 1;
	if (y_i >= 0 && y_i < colsSize && !visitBoard[z][x][y_i] && board[z][x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
	}
	z_i = z - 1;
	if (z_i >= 0 && z_i < depthSize && !visitBoard[z_i][x][y] && board[z_i][x][y] == shipChar)
	{
		shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
	}
}

bool GameData::isSpacesAreOK(int x, int y, int z, char c) const
{
	if (!checkSpacesInPosition(x + 1, y, z, c)) { return false; }
	if (!checkSpacesInPosition(x - 1, y, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y + 1, z, c)) { return false; }
	if (!checkSpacesInPosition(x, y - 1, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y, z + 1, c)) { return false; }
	if (!checkSpacesInPosition(x, y, z - 1, c)) { return false; }

	return true;
}

bool GameData::checkSpacesInPosition(int x, int y, int z, char c) const
{
	if (x > 0 && y > 0 && z > 0 && x < rowsSize && y < colsSize && z < depthSize && board[z][x][y] != c && board[z][x][y] != ' ') {
		return false;
	}
	return true;
}

GameData::~GameData()
{
	cout << "dis" << endl;
	if (board != nullptr) {
		for (int i = 0; i < depthSize; i++) {
			delete[] board[i];
		}
		delete[] board;
	}
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
