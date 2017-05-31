#include "GameManager.h"

bool GameManager::loadBoard(string boardPath)
{
	ifstream borderFile(boardPath);

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	streambuf* borderStream = borderFile.rdbuf();

	// Get board sizes
	string sizesLine;
	istream isBoard(borderStream);
	getline(isBoard, sizesLine);

	depthSize = 0;
	rowsSize = 0;
	colsSize = 0;

	board = new string*[depthSize];
	string emptyLine;
	for (auto depth = 0; depth < depthSize; depth++) {
		getline(isBoard, emptyLine);
		board = new string*[rowsSize];
		for (auto row = 0; row < rowsSize; row++) {
			auto endOfLine = false;
			string line = "";
			if (borderStream->sgetc() == EOF)
			{
				line.append(string(colsSize, ' '));
				line += '\0';
				board[depth][row] = line;
				continue;
			}
			while (!endOfLine) {
				char c = borderStream->sbumpc();
				switch (c) {
				case '\n':
					// in case it's end of file '\n'
					if (static_cast<int> (line.size()) < colsSize)
					{
						//case the line is shorter then boarder size
						size_t spaceSize = colsSize - line.size();
						line.append(string(spaceSize, ' '));
					}
					line += '\0';
					board[depth][row] += line;
					endOfLine = true;
					break;
				case '\r':
					if (borderStream->sgetc() == '\n') {
						// in case it's end of file '\r\n'
						borderStream->sbumpc();
						size_t lineSize = line.size();
						if (static_cast<int> (lineSize) < colsSize)
						{
							//case the line is shorter then boarder size
							size_t spaceSize = colsSize - line.size();
							line.append(string(spaceSize, ' '));
						}
						line += '\0';
						board[depth][row] += line;
						endOfLine = true;
						break;
					}
				default:
					// take only 'sizeBoard' first chars
					if (static_cast<int> (line.size()) < colsSize) {
						if (!isKnownLetter(c)) {
							c = ' ';
						}
						line += c;
					}
				}
			}
		}
	}
	borderFile.close();
}



bool GameManager::isKnownLetter(char c) {

	return (c == InflatableBoat::symbolAPlayer || c == InflatableBoat::symbolBPlayer ||
		c == missileBoat::symbolAPlayer || c == missileBoat::symbolBPlayer ||
		c == submarine::symbolAPlayer || c == submarine::symbolBPlayer ||
		c == battleship::symbolAPlayer || c == battleship::symbolBPlayer);
}

bool GameManager::validateBoard()
{
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

	if (fleetA.getNumberOfShips() > NUMBR_OF_SHIPS)
	{
		cout << TOO_MANY_A;
		checksPass = false;
	}
	if (fleetA.getNumberOfShips() < NUMBR_OF_SHIPS)
	{
		cout << TOO_FEW_A;
		checksPass = false;
	}
	if (fleetB.getNumberOfShips() > NUMBR_OF_SHIPS)
	{
		cout << TOO_MANY_B;
		checksPass = false;
	}
	if (fleetB.getNumberOfShips() < NUMBR_OF_SHIPS)
	{
		cout << TOO_FEW_B;
		checksPass = false;
	}

	if (adjacentError)
	{
		cout << ADJACENT;
		checksPass = false;
	}

	return checksPass;
}

void GameManager::buildShip( int x, int y, int z, char shipChar, bool*** visitBoard, list<char>& failedCharA, list<char>& failedCharB)
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


void GameManager::shipCollectChars(int x, int y, int z, char shipChar, bool*** visitBoard, Ship& ship) const {
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

bool GameManager::isSpacesAreOK(int x, int y, int z, char c) const
{
	if (!checkSpacesInPosition(x + 1, y, z, c)) { return false; }
	if (!checkSpacesInPosition(x - 1, y, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y + 1, z, c)) { return false; }
	if (!checkSpacesInPosition(x, y - 1, z, c)) { return false; }

	if (!checkSpacesInPosition(x, y, z + 1, c)) { return false; }
	if (!checkSpacesInPosition(x, y, z - 1, c)) { return false; }

	return true;
}

bool GameManager::checkSpacesInPosition(int x, int y, int z, char c) const
{
	if (x > 0 && y > 0 && z > 0 && x < rowsSize && y < colsSize && z < depthSize && board[z][x][y] != c && board[z][x][y] != ' ') {
		return false;
	}
	return true;
}

void GameManager::setAlgoA(IBattleshipGameAlgo * algo)
{
	this->playerAlgoA = algo;
}

void GameManager::setAlgoB(IBattleshipGameAlgo * algo)
{
	this->playerAlgoB = algo;
}

IBattleshipGameAlgo * GameManager::getAlgoA()
{
	return this->playerAlgoA;
}

IBattleshipGameAlgo * GameManager::getAlgoB()
{
	return this->playerAlgoB;
}
