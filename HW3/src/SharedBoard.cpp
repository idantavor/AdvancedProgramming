#include "SharedBoard.h"


bool SharedBoard::loadBoard()
{
	ifstream borderFile(boardPath);
	string line;

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	streambuf* borderStream = borderFile.rdbuf();

	Logger logger("SharedBoard");
	if (!handleSizesLine(borderStream)) {
		DIMENSIONS_INCORRECT(boardPath, logger);
		return false;
	}

	line = getNextLine(borderStream);
	if (!line.empty()) {
		FORMAT_INCORRECT(boardPath, logger);
		return false;
	}

	initializeBoard();

	bool endOfBoard = false;
	bool firstBoard = true;
	unsigned int countRowsInBord = 0;
	unsigned int depth = 0;
	bool emptyLine = false;

	while (depth < depthSize) {
		if (endOfFile) {
			emptyLine = true;
		}
		else {
			line = getNextLine(borderStream);
			emptyLine = line.empty();
		}


		//first time if there are empty Lines
		if (firstBoard && !endOfFile) {
			if (emptyLine) {
				continue;
			}
			else {
				firstBoard = false;
			}
		}

		if (countRowsInBord >= rowsSize && !emptyLine) {
			continue;
		}

		if (emptyLine) {
			if (!endOfBoard) {
				if (countRowsInBord < rowsSize - 1) {
					//first check case board lines is over and the rows is shorter the rowSize, add spaces rows
					addRowsToBoard(depth, countRowsInBord);
				}
				countRowsInBord = 0;
				depth++;
				endOfBoard = true;
			}
			if (endOfFile) {
				break;
			}
			else {
				continue;
			}
		}

		// line is not empty
		endOfBoard = false;

		// add line in to board
		addLineToBoard(line, depth, countRowsInBord);
		countRowsInBord++;
	}


	addTableToBoard(depth);

	// add table if missing

	borderFile.close();

	return true;
}

string SharedBoard::getNextLine(streambuf* borderStream) {
	string line = "";
	while (true) {
		char c = borderStream->sbumpc();
		// read until line is ended
		if (c == '\n') {
			break;
		}
		else if (c == EOF) {
			endOfFile = true;
			break;
		}
		else if (c == '\t' && borderStream->sgetc() == '\n') {
			char c = borderStream->sbumpc();
			break;
		}
		else {
			line += c;
		}
	}

	return line;
}
void SharedBoard::addRowsToBoard(unsigned int  depth, unsigned int countRowsInBord) {

	string line;
	while (countRowsInBord < rowsSize) {
		line = "";
		line.append(string(colsSize, SPACE));
		board[depth][countRowsInBord] = line;
		line = "";
		countRowsInBord++;
	}
}

void SharedBoard::addTableToBoard(unsigned int depth) {

	string line;
	while (depth < depthSize) {
		board[depth] = new string[rowsSize];
		for (unsigned int  row = 0; row < rowsSize; row++) {
			line = "";
			line.append(string(colsSize, SPACE));
			board[depth][row] = line;
		}
		depth++;
	}
}

void SharedBoard::addLineToBoard(string& line, unsigned int depth, unsigned int countRowsInBord) {

	string rowLine;
	unsigned int charIndex = 0;
	char c;
	while (charIndex < rowsSize && charIndex < line.size()) {
		c = line[charIndex];
		if (!isKnownLetter(c)) {
			c = SPACE;
		}
		rowLine += c;
		charIndex++;
	}

	// append with spaces if the line is shorter then col size
	if (rowLine.size() < colsSize)
	{
		//case the line is shorter then boarder size
		size_t spaceSize = colsSize - rowLine.size();
		rowLine.append(string(spaceSize, SPACE));

	}
	board[depth][countRowsInBord] = rowLine;
}

void SharedBoard::initializeBoard() {

	board = new string*[depthSize];
	for (unsigned int  depth = 0; depth < depthSize; depth++) {
		board[depth] = new string[rowsSize];
	}
}

bool SharedBoard::addCharToString(string& line, char c) {

	if (line.size() < colsSize)
	{
		line += c;
		return true;
	}

	return false;
}

bool SharedBoard::handleSizesLine(streambuf* borderStream) {
	// Get board sizes
	string size = "";
	vector<string> sizes;
	bool endOfLine = false;
	bool endOfSize = false;
	char c;

	//Check first line (sizes)
	string line = getNextLine(borderStream);
	for (unsigned int  i = 0; i < line.size(); i++) {
		c = line[i];
		if (isdigit(c)) {
			size += c;
		}
		else if (c == 'x' || c == 'X') {
			sizes.push_back(size);
			size = "";
		}
		else {
			return false;
		}
	}


	if (!size.empty()) {
		sizes.push_back(size);
	}
	if (sizes.size() != 3) {
		return false;
	}

	try {
		colsSize = stoi(sizes[0], nullptr, 0);
		rowsSize = stoi(sizes[1], nullptr, 0);
		depthSize = stoi(sizes[2], nullptr, 0);
	}
	catch (const invalid_argument&) {
		return false;
	}
	catch (const out_of_range&) {
		return false;
	}

	return true;
}
bool SharedBoard::isKnownLetter(char c) {

	return (c == InflatableBoat::symbolAPlayer || c == InflatableBoat::symbolBPlayer ||
		c == missileBoat::symbolAPlayer || c == missileBoat::symbolBPlayer ||
		c == submarine::symbolAPlayer || c == submarine::symbolBPlayer ||
		c == battleship::symbolAPlayer || c == battleship::symbolBPlayer);
}

SharedBoard::SharedBoard(string boardPath)
{
	this->boardPath = boardPath;
}

SharedBoard::~SharedBoard()
{
	if (board != nullptr) {
		for (unsigned int  i = 0; i < depthSize; i++) {
			delete[] board[i];
		}
		delete[] board;
	}
}

unsigned int  SharedBoard::getRowSize()
{
	return rowsSize;
}

unsigned int  SharedBoard::getDepthSize()
{
	return depthSize;
}

unsigned int  SharedBoard::getColSize()
{
	return colsSize;
}

char SharedBoard::getCharAt(int x, int y, int z)
{
	return board[z][x][y];
}
