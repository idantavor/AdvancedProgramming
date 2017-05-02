#pragma once

using namespace std;

class Utility{
public:
	static pair<bool,string> getCommandLineArg(const string arg_to_find, int argc, char* argv[]);
	static void getFileNamesFromDir(std::vector<string>& out, const string& directory);
	static void printFinishMsg(int scoreA, int scoreB, int winner);
	static bool dirExists(const std::string& dirName_in);
	static bool has_suffix(const std::string& str, const std::string& suffix);
};


