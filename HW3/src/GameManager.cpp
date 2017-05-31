#include "GameManager.h"

bool GameManager::loadBoard(std::list<string> boardPath) const
{
	ifstream borderFile(boardPath);

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	streambuf* borderStream = borderFile.rdbuf();

	// Get board sizes
	string sizesLine;
	istream isBoard(borderStream);
	getline(isBoard, sizesLine);



	for (auto column = 0; column < boardSize; column++) {
		auto endOfLine = false;
		string line = "";
		if (borderStream->sgetc() == EOF)
		{
			line.append(string(boardSize, ' '));
			line += '\0';
			board[column] = line;
			continue;
		}
		while (!endOfLine) {
			char c = borderStream->sbumpc();
			switch (c) {
			case '\n':
				// in case it's end of file '\n'
				if (static_cast<int> (line.size()) < boardSize)
				{
					//case the line is shorter then boarder size
					size_t spaceSize = boardSize - line.size();
					line.append(string(spaceSize, ' '));
				}
				line += '\0';
				board[column] += line;
				endOfLine = true;
				break;
			case '\r':
				if (borderStream->sgetc() == '\n') {
					// in case it's end of file '\r\n'
					borderStream->sbumpc();
					size_t lineSize = line.size();
					if (static_cast<int> (lineSize) < boardSize)
					{
						//case the line is shorter then boarder size
						size_t spaceSize = boardSize - line.size();
						line.append(string(spaceSize, ' '));
					}
					line += '\0';
					board[column] += line;
					endOfLine = true;
					break;
				}
			default:
				// take only 'sizeBoard' first chars
				if (static_cast<int> (line.size()) < boardSize) {
					if (!isKnownLetter(c)) {
						c = ' ';
					}
					line += c;
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
	bool** visitBoard = new bool*[boardSize];
	for (int i = 0; i < boardSize; i++) {
		visitBoard[i] = new bool[boardSize];
		for (int j = 0; j< boardSize; j++) {
			visitBoard[i][j] = false;
		}
	}

	// check spaces
	bool adjacentError = false;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			char c = board[i][j];
			if (c == ' ') {
				continue;
			}
			if (!isSpacesAreOK(i, j, c)) {
				adjacentError = true;
				break;
			}
		}
	}

	//create boards and do checks
	for (int row = 0; row < boardSize; row++) {
		string boardRow = board[row];
		for (int column = 0; column < boardSize; column++)
		{
			char shipChar = boardRow[column];
			if (visitBoard[row][column]) {
				continue;
			}
			if (shipChar == ' ')
			{
				visitBoard[row][column] = true;
				continue;
			}

			buildShip(row, column, shipChar, visitBoard, failedCharA, failedCharB);
		}
	}

	//delete visitBoard;
	for (int i = 0; i < boardSize; i++) {
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

void GameManager::buildShip(int x, int y, char shipChar, bool** visitBoard, list<char>& failedCharA, list<char>& failedCharB)
{

	if (shipChar == InflatableBoat::symbolAPlayer)
	{
		InflatableBoat* boat = new InflatableBoat();
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);

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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);

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
		shipCollectChars(x, y, shipChar, visitBoard, *boat);
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


void GameManager::shipCollectChars(int x, int y, char shipChar, bool** visitBoard, Ship& ship) const {
	visitBoard[x][y] = true;
	ship.addPointToTheList(x, y);
	int x_i = x + 1;
	if (x_i >= 0 && x_i < boardSize && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, shipChar, visitBoard, ship);
	}
	int y_i = y + 1;
	if (y_i >= 0 && y_i < boardSize && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, shipChar, visitBoard, ship);
	}
	x_i = x - 1;
	if (x_i >= 0 && x_i < boardSize && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, shipChar, visitBoard, ship);
	}
	y_i = y - 1;
	if (y_i >= 0 && y_i < boardSize && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, shipChar, visitBoard, ship);
	}
}

bool GameManager::isSpacesAreOK(int i, int j, char c) const
{
	if (!checkSpacesInPosition(i + 1, j, c)) { return false; }
	if (!checkSpacesInPosition(i, j + 1, c)) { return false; }
	if (!checkSpacesInPosition(i - 1, j, c)) { return false; }
	if (!checkSpacesInPosition(i, j - 1, c)) { return false; }
	//if (!checkSpacesInPosition(i-1, j-1, c)) { return false; }
	//if (!checkSpacesInPosition(i+1, j+1, c)) { return false; }
	//if (!checkSpacesInPosition(i+1, j-1, c)) { return false; }
	//if (!checkSpacesInPosition(i-1, j+1, c)) { return false; }

	return true;
}

bool GameManager::checkSpacesInPosition(int x, int y, char c) const
{
	if (x > 0 && y > 0 && x < boardSize && y < boardSize && board[x][y] != c && board[x][y] != ' ') {
		return false;
	}
	return true;
}
