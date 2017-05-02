#include "NaiveAlgo.h"



void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//currently nothing to do with it..
}

NaiveAlgo::~NaiveAlgo()
{
	for (int i = 0; i < m_rowCount; i++) {
		delete[] m_board[i];
	}
	delete[] m_board;
}

bool NaiveAlgo::init(const std::string& path)
{
	//TODO
	return true;
}

void NaiveAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	m_player = player;
	m_rowCount = numRows;
	m_colCount = numCols;
	m_board = new char*[numRows];
	for(int i=0; i< numRows; i++)
	{
		m_board[i] = new char[numCols];
		for(int j=0; j< numCols; j++)
		{
			m_board[i][j] = board[i][j];
		}
	}
}

pair<int, int> NaiveAlgo::attack()
{
	if(firstPos)
	{
		firstPos = false;
		currentPosition.setPosition(0, 0);
		return pair<int, int>(currentPosition.getX() + 1, currentPosition.getY() + 1);
	}
	getNextPosition(currentPosition);
	return pair<int,int>(currentPosition.getX()+1, currentPosition.getY()+1);
}

void NaiveAlgo::getNextPosition(Position& position) const
{
	int xPos = position.getX();
	int yPos = position.getY();
	if(yPos < m_rowCount)
	{
		yPos++;
		position.setY(yPos);
	}
	else if(xPos < m_colCount)
	{
		xPos++;
		position.setPosition(xPos, 0);
	}
	else
	{
		position.setPosition(-2, -2);
		return;
	}
	
	if(!isPositionOk(position))
	{
		getNextPosition(position);
	}
}

bool NaiveAlgo::isPositionOk(Position position) const
{
	if(m_board[position.getX()][position.getY()] != ' ')
	{
		return false;
	}
	if (position.getX() > 0 && m_board[position.getX()-1][position.getY()] != ' ')
	{
		return false;
	}
	if (position.getX() < m_rowCount-1 && m_board[position.getX() + 1][position.getY()] != ' ')
	{
		return false;
	}
	if (position.getY() > 0 && m_board[position.getX()][position.getY()-1] != ' ')
	{
		return false;
	}
	if (position.getY() < m_colCount - 1 && m_board[position.getX()][position.getY()+1] != ' ')
	{
		return false;
	}

	return true;
}

IBattleshipGameAlgo* GetAlgorithm() {
	return new NaiveAlgo();
};