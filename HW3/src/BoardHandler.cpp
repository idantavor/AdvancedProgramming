#include "BoardHandler.h"

bool BoardHandler::loadBoard(std::list<string> boardsPath) const
{
	bool boardIsOk = false;
	for (std::list<string>::iterator it = boardsPath.begin(); it != boardsPath.end(); ++it) {
		boardIsOk = checkAndLoadBoard(*it);
		if (boardIsOk) {
			break;
		}
	}

	return boardIsOk;
}

bool BoardHandler::checkAndLoadBoard(const string& boardPath) const
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
