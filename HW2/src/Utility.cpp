#pragma once
#include "BattleManager.h"
#include "Utility.h"
#include "dirent.h"

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

void  Utility::getFileNamesFromDir(std::vector<string> &out, const string &directory) {
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

void Utility::printFinishMsg(int scoreA, int scoreB, int winner) { // winner A_TURN for A, B_TURN for B , -1 for no winner
	if (winner != -1) {
		cout << "Player " << ((winner == A_TURN) ? "A " : "B ") << "won" << endl;
	}
	cout << "Points:" << endl;
	cout << "Player A: " << scoreA << endl;
	cout << "Player B: " << scoreB << endl;
}

bool Utility::dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

bool Utility::has_suffix(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}




