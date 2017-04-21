#include "BattleShipGameFromFile.h"


//C'tor
BattleShipGameFromFile::BattleShipGameFromFile(const string& attackFilePath) {
	//here the attachFilePath should exist
	m_attackFileIFS = ifstream(attackFilePath);
	if (!m_attackFileIFS.is_open()) {
		cout << "failed to open attack file " << attackFilePath;
		throw exception("fatal error, failed to open attack file");
	}
}

//D'tor
BattleShipGameFromFile::~BattleShipGameFromFile() {
	m_attackFileIFS.close();
}

bool BattleShipGameFromFile::init(const std::string & path)
{
	return false;
}

void BattleShipGameFromFile::setBoard(int player, const char** board, int numRows, int numCols) {
	m_rowCount = numRows;
	m_colCount = numCols;
	m_player = player;
}

void BattleShipGameFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	//currently nothing to do with it..
}

std::pair<int, int> BattleShipGameFromFile::attack() {
	string line;
	auto result = std::pair<int,int>(-1,-1);
	while (getline(m_attackFileIFS, line)) {
		result = parseLine(line);
		if (result.first == PAIR_NO_MATCH.first) { // enough to check only one side for sucess/failure
			continue;
		}
		else {
			break;
		}
	}
	return result;
}


//requierd format is "123  ,    456" (row,col)
std::pair<int, int> BattleShipGameFromFile::parseLine(const string& line) {
	const std::regex txt_regex("-?[0-9]+[\\ ]*\\,[\\ ]*-?[0-9]+");
	//make sure format fits
	bool match = std::regex_match(line, txt_regex);
	if (!match) {
		return PAIR_NO_MATCH;
	}
	//try parsing the two positions
	int row =  -1;
	int col =  -1;
	std::stringstream stream(line);
	stream >> row;
	while ((stream.peek() == ' ') || (stream.peek() == ',')) {
		stream.ignore();
	}
	stream >> col;
	//make sure they are in range, and that parsing worked
	if (! ((row >= MIN_ROW) && (row <= m_rowCount)) ) {
		
		return PAIR_NO_MATCH;
	}
	if (! ((col >= MIN_COL) && (col <= m_colCount)) ) {
		return PAIR_NO_MATCH;
	}
	//success
	return std::pair<int, int>(row,col);
}


