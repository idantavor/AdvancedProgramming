#include "NaiveAlgo.h"
#include "Utility.h"


void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	
}

void NaiveAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	userFleet = Utility::setUserFleetFromBoard(board, numRows, numCols);
}