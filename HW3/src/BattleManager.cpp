#include "BattleManager.h"
#include "IBattleshipGameAlgo.h"
#include "UserFleet.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include <winapifamily.h>
#include <functional>
#include <memory>
#include <vector>
#include "dirent.h"
#include "Ship.h"
#include <string>
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "Utility.h"




IBattleshipGameAlgo* BattleManager::loadFromDLL(string path)
{	
	UINT oldMode = SetErrorMode(0);
	SetErrorMode(oldMode | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
		IBattleshipGameAlgo * instance;
		HINSTANCE hDll;
		try {
		// Load dynamic library 
		hDll = LoadLibraryA(path.c_str());
		using FunctionPtr = IBattleshipGameAlgo* (*) ();
		if (hDll)
		{
			// GetAlgorithm function
			auto GetAlgorithm = reinterpret_cast<FunctionPtr>(GetProcAddress(hDll, "GetAlgorithm"));
			instance = GetAlgorithm();
		}
		else {
			throw exception("failed to load library");
		}
	}
	catch (exception e) {
		throw e;
	}
	
	return instance;
}

bool BattleManager::isKnownLetter(char c) {

	return (c == InflatableBoat::symbolAPlayer || c == InflatableBoat::symbolBPlayer ||
		c == missileBoat::symbolAPlayer || c == missileBoat::symbolBPlayer ||
		c == submarine::symbolAPlayer || c == submarine::symbolBPlayer ||
		c == battleship::symbolAPlayer || c == battleship::symbolBPlayer);
}

bool BattleManager::validateBoard()
{
	list<char> failedCharA;
	list<char> failedCharB;

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

			buildShip(row, column, shipChar, visitBoard, failedCharA, failedCharB);
		}
	}
 
	//delete visitBoard;
	for (int i = 0; i < boardSize; i++) {
		delete[] visitBoard[i];
	}
	delete[] visitBoard;

	bool checksPass = true;

	if(!failedCharA.empty() || !failedCharB.empty())
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
	
	if(adjacentError)
	{
		cout << ADJACENT;
		checksPass = false;
	}

	return checksPass;
}

void BattleManager::buildShip(int x, int y, char shipChar, bool** visitBoard, list<char>& failedCharA, list<char>& failedCharB) 
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


void BattleManager::shipCollectChars(int x, int y, char shipChar, bool** visitBoard, Ship& ship) const{
	visitBoard[x][y] = true;
	ship.addPointToTheList(x, y);
	int x_i = x+1;
	if(x_i >= 0 && x_i < boardSize && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		shipCollectChars(x_i, y, shipChar, visitBoard, ship);
	}
	int y_i = y + 1;
	if(y_i >= 0 && y_i < boardSize && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		shipCollectChars(x, y_i, shipChar, visitBoard, ship);
	}
	x_i = x -1 ;
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
	if (searchIn != "." && !Utility::dirExists(searchIn)) {
		cout << "Wrong path : " << searchIn << endl;
		return false;
	}
	
	//get file names in dir path
	vector<string> fileNames;
	Utility::getFileNamesFromDir(fileNames, searchIn);

	for (unsigned int i = 0; i < fileNames.size(); i++) {
		if (Utility::has_suffix(fileNames[i], suffixes[BOARD_PATH])) {
			if (!isBoardFound) {
				filePaths[BOARD_PATH] = searchIn + '\\' + fileNames[i];
				isBoardFound = true;
			}
		}
		if (Utility::has_suffix(fileNames[i], suffixes[1])) {
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
		filePaths[DLL_A_PATH] = searchIn + '\\' + dllFiels[0];
		filePaths[DLL_B_PATH] = searchIn + '\\' + dllFiels[1];
	}

	if (isFileNotFound || !validBoard)return false; // file not found
	
	return true;
	
}



bool BattleManager::runBattle(const string & dirPath)
{
	int currentTurn = 0;// A turn when 0 , B turn when 1;
	if(!validateFilesExistanceAndBoardValidate(dirPath))return false; //validate all input files existanc and board validate
	// set player boards
	//create tmp char** from board to pass to function
	
	try {
		
		playerA = loadFromDLL(filePaths[DLL_A_PATH]);
	
	}
	catch (exception e) {
		cout << "Cannot load dll: " << filePaths[DLL_A_PATH] << endl;
		return false;
	}

	char** boardAchar = new char*[this->boardSize];
	char** boardBchar = new char*[this->boardSize];
	buildUserBoards(boardAchar, boardBchar);

	playerA->setBoard(0, const_cast<const char**>(boardAchar), static_cast<int> (this->boardSize), static_cast<int> (this->boardSize)); // should only contain each players board but since it is almost empty imple doesn't matter
	//free the tmp board, no need to free the char* generated by c_str(), as it points too the string which will manage it's own lifetime
	deleteBoard(boardAchar);
	if (!playerA->init(dirPath))
	{
		cout << "Algorithm initialization failed for dll: " << filePaths[DLL_A_PATH] << endl;
		deleteBoard(boardBchar);
		return false;
	}

	try {
		
		playerB = loadFromDLL(filePaths[DLL_B_PATH]);

	}
	catch (exception e) {
		cout << "Cannot load dll: " << filePaths[DLL_B_PATH] << endl;
		deleteBoard(boardBchar);
		return false;
	}
	playerB->setBoard(1, const_cast<const char**>(boardBchar), static_cast<int> (this->boardSize), static_cast<int>(this->boardSize));
	deleteBoard(boardBchar);

	if(!playerB->init(dirPath))
	{
		cout << "Algorithm initialization failed for dll: " << filePaths[DLL_B_PATH] << endl;
		return false;
	}

	if (!quiet)cp.print_borad();//print board if not quiet
																		 
	//init player's status
	bool playerAFin = false; bool playerBFin = false;
	int playerAScore = 0; int playerBScore = 0;

	while (true) {
		if (playerAFin && playerBFin) {
			Utility::printFinishMsg(playerAScore, playerBScore, -1);
			return true;
		}
		pair<int, int> currAttack = (currentTurn == A_TURN) ? playerA->attack() : playerB->attack();
		if (currAttack == pair<int, int>(-1, -1)) {
			//mark player finished , switch turns and continue
			(currentTurn == A_TURN) ? playerAFin = true : playerBFin = true;
			currentTurn = (currentTurn + 1) % 2; 
			continue;
		}
		Position posToAttack = Position(currAttack.first - 1, currAttack.second - 1);
		auto result = (currentTurn == A_TURN) ? fleetB.executeAttack(posToAttack) : fleetA.executeAttack(posToAttack);
		auto selfResult = (currentTurn == A_TURN) ? fleetA.executeAttack(posToAttack) : fleetB.executeAttack(posToAttack);
	
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
		if (!fleetA.isNotLose()) { // a lost meaning b won
			Utility::printFinishMsg(playerAScore, playerBScore, B_TURN);
			return true;
		}
		if(!fleetB.isNotLose()) { // B lost meaning A won
			Utility::printFinishMsg(playerAScore, playerBScore, A_TURN);
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

void BattleManager::deleteBoard(char** boardToDelete) const
{
	for (int i = 0; i < this->boardSize; i++) {
		delete[] boardToDelete[i];
	}
	delete[] boardToDelete;
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

	
