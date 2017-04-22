#include "BattleManager.h"
#include "IBattleshipGameAlgo.h"
#include "UserFleet.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include <vector>
#include "dirent.h"
#include "Ship.h"
#include <string>
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "NaiveAlgo.h"


bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;  
	return false;    
}

bool has_suffix(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void BattleManager::loadBoard(const string& boardPath) const
{

	ifstream borderFile(boardPath);

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	streambuf* borderStream = borderFile.rdbuf();
	for (auto column = 0; column < boardSize; column++) {
		auto endOfLine = false;
		string line= "";
		if (borderStream->sgetc() == EOF)
		{
			line.append(string(boardSize, ' '));
			line += '\0';
			board[column]= line;
			continue;
		}
		while (!endOfLine) {
			char c = borderStream->sbumpc();
			switch (c) {
			case '\n':
				// in case it's end of file '\n'

				if (line.size() < boardSize)
				{
					//case the line is shorter then boarder size
					int spaceSize = boardSize - line.size();
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
					int lineSize = line.size();
					if (lineSize < boardSize)
					{
						//case the line is shorter then boarder size
						int spaceSize = boardSize - line.size();
						line.append(string(spaceSize, ' '));
					}
					line += '\0';
					board[column] += line;
					endOfLine = true;
					break;
				}
			default:
				// take only 'sizeBoard' first chars
				if (line.size() < boardSize) {
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

bool BattleManager::isKnownLetter(char c){

	if (c == InflatableBoat::symbolAPlayer || c == InflatableBoat::symbolBPlayer ||
		c == missileBoat::symbolAPlayer  || c == missileBoat::symbolBPlayer ||
		c == submarine::symbolAPlayer || c == submarine::symbolBPlayer ||
		c == battleship::symbolAPlayer || c == battleship::symbolBPlayer)
	{
		return true;
	}

	return false;
}

bool BattleManager::validateBoard()
{
	list<Ship*>* shipsListA = new list<Ship*>();
	list<Ship*>* shipsListB = new list<Ship*>();
	list<char>* failedCharA = new list<char>();
	list<char>* failedCharB = new list<char>();

	//Insilize visit board
	bool** visitBoard = new bool*[boardSize];
	for (int i = 0; i < boardSize; i++) {
		visitBoard[i] = new bool[boardSize];
		for(int j=0 ; j< boardSize; j++){
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

			buildShip(row, column, shipChar, visitBoard, shipsListA, shipsListB, failedCharA, failedCharB);
		}
	}
 
	//delete visitBoard;
	for (int i = 0; i < boardSize; i++) {
		delete[] visitBoard[i];
	}
	delete[] visitBoard;

	fleetA = new UserFleet(shipsListA);
	fleetB = new UserFleet(shipsListB);

	bool checksPass = true;

	if(!failedCharA->empty() || !failedCharB->empty())
	{
		checksPass = false;
		
	}
	for (std::list<char>::iterator it = (*failedCharA).begin(); it != (*failedCharA).end(); ++it)
	{
		WRONG_SIZE_A(*it);
	}
	for (std::list<char>::iterator it = (*failedCharB).begin(); it != (*failedCharB).end(); ++it)
	{
		WRONG_SIZE_B(*it);
	}

	if (fleetA->getNumberOfShips() > NUMBR_OF_SHIPS)
	{
		cout << TOO_MANY_A;
		checksPass = false;
	}
	if (fleetA->getNumberOfShips() < NUMBR_OF_SHIPS)
	{
		cout << TOO_FEW_A;
		checksPass = false;
	}
	if (fleetB->getNumberOfShips() > NUMBR_OF_SHIPS)
	{
		cout << TOO_MANY_B;
		checksPass = false;
	}
	if (fleetB->getNumberOfShips() < NUMBR_OF_SHIPS)
	{
		cout << TOO_FEW_B;
		checksPass = false;
	}

	if(adjacentError)
	{
		cout << ADJACENT;
		checksPass = false;
	}
	delete failedCharA;
	delete failedCharB;
	delete shipsListA;
	delete shipsListB;

	return checksPass;
}

void BattleManager::buildShip(int x, int y, char shipChar, bool** visitBoard, list<Ship*>* shipsListA,
	list<Ship*>* shipsListB, list<char>* failedCharA, list<char>* failedCharB) const
{

	if (shipChar == InflatableBoat::symbolAPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		InflatableBoat* boat = new InflatableBoat(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharA).begin(), (*failedCharA).end(), InflatableBoat::symbolAPlayer) != (*failedCharA).end());
			if (!found) {
				failedCharA->push_back(InflatableBoat::symbolAPlayer);
			}
			delete boat;
		}
		else
		{
			shipsListA->push_back(boat);
		}
	}
	else if (shipChar == InflatableBoat::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		InflatableBoat* boat = new InflatableBoat(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharB).begin(), (*failedCharB).end(), InflatableBoat::symbolBPlayer) != (*failedCharB).end());
			if (!found) {
				failedCharB->push_back(InflatableBoat::symbolBPlayer);
			}
			delete boat;
		}
		else
		{
			shipsListB->push_back(boat);
		}
	}
	else if (shipChar == missileBoat::symbolAPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		missileBoat* boat = new missileBoat(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{

			bool found = (std::find((*failedCharA).begin(), (*failedCharA).end(), missileBoat::symbolAPlayer) != (*failedCharA).end());
			if (!found) {
				failedCharA->push_back(missileBoat::symbolAPlayer);
			}
			delete boat;
		}
		else
		{
			shipsListA->push_back(boat);
		}
	}
	else if (shipChar == missileBoat::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		missileBoat* boat = new missileBoat(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharB).begin(), (*failedCharB).end(), missileBoat::symbolBPlayer) != (*failedCharB).end());
			if (!found) {
				failedCharB->push_back(missileBoat::symbolBPlayer);
			}
			delete boat;
		}
		else {
			shipsListB->push_back(boat);
		}
	}
	else if (shipChar == submarine::symbolAPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		submarine* boat = new submarine(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharA).begin(), (*failedCharA).end(), submarine::symbolAPlayer) != (*failedCharA).end());
			if (!found) {
				failedCharA->push_back(submarine::symbolAPlayer);
			}
			delete boat;
		}
		else {
			shipsListA->push_back(boat);
		}
	}
	else if (shipChar == submarine::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		submarine* boat = new submarine(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharB).begin(), (*failedCharB).end(), submarine::symbolBPlayer) != (*failedCharB).end());
			if (!found) {
				failedCharB->push_back(submarine::symbolBPlayer);
			}
			delete boat;
		}
		else {
			shipsListB->push_back(boat);
		}
	}
	else if (shipChar == battleship::symbolAPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		battleship* boat = new battleship(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharA).begin(), (*failedCharA).end(), battleship::symbolAPlayer) != (*failedCharA).end());
			if (!found) {
				failedCharA->push_back(battleship::symbolAPlayer);
			}
			delete boat;
		}
		else {
			shipsListA->push_back(boat);
		}
	}
	else if (shipChar == battleship::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		shipCollectChars(x, y, shipChar, visitBoard, positionList);
		battleship* boat = new battleship(positionList);
		bool isOk = (*boat).checkShape();
		if (!isOk)
		{
			bool found = (std::find((*failedCharB).begin(), (*failedCharB).end(), battleship::symbolBPlayer) != (*failedCharB).end());
			if (!found) {
				failedCharB->push_back(battleship::symbolBPlayer);
			}
			delete boat;
		}
		else {
			shipsListB->push_back(boat);
		}
	}
}


void BattleManager::shipCollectChars(int x, int y, char shipChar, bool** visitBoard, list<Position>* positionList) const{
	visitBoard[x][y] = true;
	(*positionList).push_back(Position(x, y));
	int x_i = x+1;
	if(x_i >= 0 && x_i < boardSize && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, shipChar, visitBoard, positionList);
	}
	int y_i = y + 1;
	if(y_i >= 0 && y_i < boardSize && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, shipChar, visitBoard, positionList);
	}
	x_i = x -1 ;
	if (x_i >= 0 && x_i < boardSize && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, shipChar, visitBoard, positionList);
	}
	y_i = y - 1;
	if (y_i >= 0 && y_i < boardSize && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, shipChar, visitBoard, positionList);
	}
}
bool BattleManager::validateFilesExistanceAndBoardValidate(const std::string& dirPath) {
	vector<string> suffixes = { "sboard", "dll" }; // file suffixes to validate
	vector<string> errors = { "Missing board file (*.sboard) looking in path: ", "Missing an algorithm (dll) file looking in path: " };
	vector<string> dllFiels;
	bool isBoardFound = false;
	bool validBoard = false;
	filePaths = vector<string>(3, "");
	bool isFileNotFound = false;
	string searchIn;
	
	//check if dirPath provided
	if (dirPath.empty()) {
		searchIn = ".";
	}
	else {
		searchIn = dirPath;
	}
	
	//if dirPath provided check if dir exists
	if (searchIn != "." && !dirExists(searchIn)) {
		cout << "Wrong path : " << searchIn << endl;
		return false;
	}
	
	//get file names in dir path
	vector<string> fileNames;
	getFileNamesFromDir(fileNames, searchIn);

	for (unsigned int i = 0; i < fileNames.size(); i++) {
		if (has_suffix(fileNames[i], suffixes[BOARD_PATH])) {
			filePaths[BOARD_PATH] = searchIn + '\\' + fileNames[i];
			isBoardFound = true;
		}
		if (has_suffix(fileNames[i], suffixes[1])) {
			dllFiels.push_back(fileNames[i]);
		}
	}

	if(!isBoardFound)
	{
		cout << errors[BOARD_PATH] << searchIn << endl;
		isFileNotFound = true;
	}
	else
	{
		loadBoard(filePaths[BOARD_PATH]); //load and 
		validBoard = validateBoard(); //validate the board
	}

	if(dllFiels.size() <2 ){
		cout << errors[DLL_A_PATH] << searchIn << endl;
		isFileNotFound = true;
	}
	else
	{
		sort(dllFiels.begin(), dllFiels.end());
		filePaths[DLL_A_PATH] = searchIn + '\\' + fileNames[0];
		filePaths[DLL_B_PATH] = searchIn + '\\' + fileNames[1];
	}

	if (isFileNotFound || !validBoard)return false; // file not found
	
	return true;
	
}



bool BattleManager::runBattle(const string & dirPath)
{
	int currentTurn = 0;// A turn when 0 , B turn when 1;
	if(!validateFilesExistanceAndBoardValidate(dirPath))return false; //validate all input files existanc and board validate
	this->cp.setBoard(this->board, this->boardSize);//set console printer board
	// set player boards
	//create tmp char** from board to pass to function

	if (!quiet)cp.print_borad();//print board if not quiet
	
	//TODO take from dll
	try {
		// initialize the player A
		//playerA = new NaiveAlgo();
		playerA = new BattleShipGameFromFile();
	}
	catch (exception e) {
		cout << "ERROR: initing player A" << endl;
		return false;
	}

	char** boardAchar = new char*[this->boardSize];
	char** boardBchar = new char*[this->boardSize];
	buildUserBoards(boardAchar, boardBchar);

	playerA->setBoard(0, const_cast<const char**>(boardAchar), this->boardSize, this->boardSize); // should only contain each players board but since it is almost empty imple doesn't matter
	//free the tmp board, no need to free the char* generated by c_str(), as it points too the string which will manage it's own lifetime
	delete[] boardAchar;
	if (!playerA->init(dirPath))
	{
		cout << "Algorithm initialization failed for dll: " << filePaths[DLL_A_PATH] << endl;
		delete[] boardBchar;
		return false;
	}

	//TODO load dll2
	try {
		// initialize the players
		//playerB = new NaiveAlgo();
		playerB = new BattleShipGameFromFile();
	}
	catch (exception e) {
		cout << "ERROR: initing player B" << endl;
		delete[] boardBchar;
		return false;
	}
	playerB->setBoard(1, const_cast<const char**>(boardBchar), this->boardSize, this->boardSize);
	delete[] boardBchar;

	if(!playerB->init(dirPath))
	{
		cout << "Algorithm initialization failed for dll: " << filePaths[DLL_B_PATH] << endl;
		return false;
	}
																		 
	//init player's status
	bool playerAFin = false; bool playerBFin = false;
	int playerAScore = 0; int playerBScore = 0;

	while (true) {
		if (playerAFin && playerBFin) {
			printFinishMsg(playerAScore, playerBScore, -1);
			return true;
		}
		pair<int, int> currAttack = (currentTurn == A_TURN) ? playerA->attack() : playerB->attack();
		if (currAttack == pair<int, int>(-1, -1)) {
			//mark player finished , switch turns and continue
			(currentTurn == A_TURN) ? playerAFin = true : playerBFin = true;
			currentTurn = (currentTurn + 1) % 2; 
			continue;
		}
		//execute attack and get result
		if (currentTurn == A_TURN) {
			if (currAttack.first == 10) {

			}
		}
		Position posToAttack = Position(currAttack.first - 1, currAttack.second - 1);
		auto result = (currentTurn == A_TURN) ? fleetB->executeAttack(posToAttack) : fleetA->executeAttack(posToAttack);
		auto selfResult = (currentTurn == A_TURN) ? fleetA->executeAttack(posToAttack) : fleetB->executeAttack(posToAttack);
		//std::pair<AttackResult, Ship*> result;
		//std::pair<AttackResult, Ship*> selfResult;
		//if (currentTurn == A_TURN) {
		//	result = fleetB->executeAttack(posToAttack);
		//	selfResult = fleetA->executeAttack(posToAttack);
		//}
		//else {
		//	selfResult = fleetB->executeAttack(posToAttack);
		//	result = fleetA->executeAttack(posToAttack);
		//}

		AttackResult unifiedRes = AttackResult::Miss;
		bool shouldSwitchTurn = false;
		switch (result.first) {
			case AttackResult::Miss ://switch turns 
				shouldSwitchTurn = true;
				break;
			case AttackResult::Hit : 
				unifiedRes = AttackResult::Hit;
				break;
			case AttackResult::Sink ://update score
				unifiedRes = AttackResult::Sink;
				(currentTurn == A_TURN) ? playerAScore += result.second->getScore() : playerBScore += result.second->getScore();
				break;
		}
		//if I Missed opponent might have hit myself
		if (result.first == AttackResult::Miss) {
			switch (selfResult.first) {
				case AttackResult::Miss:
					unifiedRes = AttackResult::Miss;
					break;
				case AttackResult::Hit:
					unifiedRes = AttackResult::Hit;
					break;
				case AttackResult::Sink://update score
					unifiedRes = AttackResult::Sink;
					(currentTurn == A_TURN) ? playerBScore += selfResult.second->getScore() : playerAScore += selfResult.second->getScore();
					break;
			}
		}
		playerA->notifyOnAttackResult(currentTurn, currAttack.first, currAttack.second, unifiedRes); //notify A
		playerB->notifyOnAttackResult(currentTurn, currAttack.first, currAttack.second, unifiedRes); //notify B
		if (!quiet)cp.print_attack(posToAttack, currentTurn, unifiedRes);//print attack to console if not quiet
		//check game status and report accordingly
		if (!fleetA->isNotLose()) { // a lost meaning b won
			printFinishMsg(playerAScore, playerBScore, B_TURN);
			return true;
		}
		if(!fleetB->isNotLose()) { // B lost meaning A won
			printFinishMsg(playerAScore, playerBScore, A_TURN);
			return true;
		}
		if (shouldSwitchTurn) {
			currentTurn = (currentTurn + 1) % 2;
		}
		continue;
	}

	return true;

}

void BattleManager::buildUserBoards(char** boardA, char** boardB) const
{
	for (int i = 0; i < this->boardSize; i++) {
		boardA[i] = new char[this->boardSize];
		boardB[i] = new char[this->boardSize];
		for (int j = 0; j < this->boardSize; j++) {
			if(this->board[i][j] == ' ')
			{
				boardA[i][j] = ' ';
				boardB[i][j] = ' ';
				continue;
			}
			if (this->board[i][j] >= 'A' && this->board[i][j] <= 'Z') {
				boardA[i][j] = this->board[i][j];
				boardB[i][j] = ' ';
			}
			else
			{
				boardA[i][j] = ' ';
				boardB[i][j] = this->board[i][j];
			}
		}
	}
}


bool BattleManager::isSpacesAreOK(int i, int j, char c) const
{
	if (!checkSpacesInPosition(i + 1, j, c)) { return false; }
	if (!checkSpacesInPosition(i, j+1 , c)) { return false; }
	if (!checkSpacesInPosition(i-1, j, c)) { return false; }
	if (!checkSpacesInPosition(i, j-1, c)) { return false; }
	//if (!checkSpacesInPosition(i-1, j-1, c)) { return false; }
	//if (!checkSpacesInPosition(i+1, j+1, c)) { return false; }
	//if (!checkSpacesInPosition(i+1, j-1, c)) { return false; }
	//if (!checkSpacesInPosition(i-1, j+1, c)) { return false; }

	return true;
}

bool BattleManager::checkSpacesInPosition(int x, int y, char c) const
{
	if (x > 0 && y > 0 && x < boardSize && y < boardSize && board[x][y] != c && board[x][y] != ' ') {
		return false;
	}
	return true;
}


void getFileNamesFromDir(std::vector<string> &out, const string &directory) {
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

void printFinishMsg(int scoreA, int scoreB, int winner) { // winner A_TURN for A, B_TURN for B , -1 for no winner
	if (winner != -1) {
		cout << "Player " << ((winner == A_TURN) ? "A " : "B " )<< "won" << endl;
	}
	cout << "Points:" << endl;
	cout << "Player A: " << scoreA << endl;
	cout << "Player B: " << scoreB << endl;
}
	
