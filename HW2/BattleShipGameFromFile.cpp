#include "BattleShipGameFromFile.h"
#include "dirent.h"


BattleShipGameFromFile::BattleShipGameFromFile()
{
}

//D'tor
BattleShipGameFromFile::~BattleShipGameFromFile() {
	m_attackFileIFS.close();
}

bool BattleShipGameFromFile::init(const std::string & path)
{

	//get file names in dir path
	vector<string> fileNames;
	string searchIn;

	//check if dirPath provided
	if (path.empty()) {
		searchIn = ".";
	}
	else {
		searchIn = path;
	}
	getFileNamesFromDir(fileNames, searchIn);
	vector<string> attackFiles;

	for (unsigned int i = 0; i < fileNames.size(); i++) {
		if (has_suffix(fileNames[i], "attack")) {
			attackFiles.push_back(fileNames[i]);
		}
	}

	if(attackFiles.size() == 0)
	{
		return false;
	}

	string attackPath;
	if(attackFiles.size() == 1)
	{
		attackPath = searchIn + '\\' + attackFiles[m_player];
	}
	else
	{
		sort(attackFiles.begin(), attackFiles.end());
		attackPath = searchIn + '\\' + attackFiles[m_player];
	}
	//here the attachFilePath should exist
	m_attackFileIFS = ifstream(attackPath);
	if (!m_attackFileIFS.is_open()) {
		return false;
		/*cout << "failed to open attack file " << path;
		throw exception("fatal error, failed to open attack file");*/
	}

	return true;
}

void BattleShipGameFromFile::setBoard(int player, const char** board, int numRows, int numCols) {
	m_rowCount = numRows;
	m_colCount = numCols;
	m_player = player;
}

void  BattleShipGameFromFile :: getFileNamesFromDir(std::vector<string> &out, const string &directory) {
	DIR *dpdf;
	struct dirent *epdf;
	dpdf = opendir(directory.c_str());
	if (dpdf != NULL) {
		while (epdf = readdir(dpdf)) {
			if (string(epdf->d_name) == "." || string(epdf->d_name) == "..")continue;
			out.push_back(epdf->d_name);
		}
		closedir(dpdf);
	}
}

bool BattleShipGameFromFile::has_suffix(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
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


