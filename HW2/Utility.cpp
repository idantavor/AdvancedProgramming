#pragma once
#include "BattleManager.h"
#include "UserFleet.h"
#include <windows.h>
#include "Ship.h"
#include "InflatableBoat.h"
#include "battleship.h"
#include "submarine.h"
#include "missileBoat.h"
#include "Utility.h"

UserFleet* Utility::setUserFleetFromBoard(const char** board, int numRows, int numCols){
	list<Ship*>* shipsList = new list<Ship*>();

	//Insilize visit board
	bool** visitBoard = new bool*[numRows];
	for (int i = 0; i < numRows; i++) {
		visitBoard[i] = new bool[numCols];
		for (int j = 0; j< numCols; j++) {
			visitBoard[i][j] = false;
		}
	}

	//create boards and do checks
	for (int row = 0; row < numRows; row++) {
		string boardRow = board[row];
		for (int column = 0; column < numCols; column++)
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

			buildUserShip(board, row, column, shipChar, visitBoard, shipsList, numRows, numCols);
		}
	}

	//delete visitBoard;
	for (int i = 0; i < numRows; i++) {
		delete[] visitBoard[i];
	}
	delete[] visitBoard;

	UserFleet* fleet = new UserFleet(shipsList);

	delete shipsList;

	return fleet;
}

void Utility::buildUserShip(const char** board, int x, int y, char shipChar, bool** visitBoard, list<Ship*>* shipsList, int numRows, int numCols)
{

	if (shipChar == InflatableBoat::symbolAPlayer || shipChar == InflatableBoat::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		collectShipChars(board,x, y, shipChar, visitBoard, positionList, numRows,numCols);
		InflatableBoat* boat = new InflatableBoat(positionList);
		shipsList->push_back(boat);
	}
	else if (shipChar == missileBoat::symbolAPlayer || shipChar == missileBoat::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		collectShipChars(board, x, y, shipChar, visitBoard, positionList, numRows, numCols);
		missileBoat* boat = new missileBoat(positionList);
		shipsList->push_back(boat);	
	}
	else if (shipChar == submarine::symbolAPlayer || shipChar == submarine::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		collectShipChars(board, x, y, shipChar, visitBoard, positionList, numRows, numCols);
		submarine* boat = new submarine(positionList);
		shipsList->push_back(boat);
	}
	else if (shipChar == battleship::symbolAPlayer || shipChar == battleship::symbolBPlayer)
	{
		list<Position>* positionList = new list<Position>();
		collectShipChars(board, x, y, shipChar, visitBoard, positionList, numRows, numCols);
		battleship* boat = new battleship(positionList);
		shipsList->push_back(boat);
	}
}

void Utility::collectShipChars(const char** board, int x, int y, char shipChar, bool ** visitBoard, list<Position>* positionList, int numRows, int numCols)
{
	visitBoard[x][y] = true;
	(*positionList).push_back(Position(x, y));
	int x_i = x + 1;
	if (x_i > 0 && x_i < numRows && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		collectShipChars(board, x_i, y, shipChar, visitBoard, positionList, numRows, numCols);
	}
	int y_i = y + 1;
	if (y_i > 0 && y_i < numCols && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		collectShipChars(board, x, y_i, shipChar, visitBoard, positionList, numRows, numCols);
	}
	x_i = x - 1;
	if (x_i > 0 && x_i < numRows && !visitBoard[x_i][y] && board[x_i][y] == shipChar)
	{
		collectShipChars(board, x_i, y, shipChar, visitBoard, positionList, numRows, numCols);
	}
	y_i = y - 1;
	if (y_i > 0 && y_i < numCols && !visitBoard[x][y_i] && board[x][y_i] == shipChar)
	{
		collectShipChars(board, x, y_i, shipChar, visitBoard, positionList, numRows, numCols);
	}
}

pair<bool, string> Utility::getCommandLineArg(const string arg_to_find, int argc, char * argv[])
{
	auto result = pair<bool,string>(false, "");
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if ( arg == "-" + arg_to_find) {
			result.first = true;
			if (i + 1 >= argc) {
				return result;
			}
			string val = argv[i + 1];
			if (val.length() == 0) {
				return result;
			}
			else {
				if (val[0] == '-')return result;
				else {
					result.second = val;
					return result;
				}
			}
			
		}
	}
	return result;
}


