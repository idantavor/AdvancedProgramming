#pragma once
#include <stdio.h>
#include <time.h>
#include <string>
#include <mutex>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
class InnerLogger {

private:
	mutex logWriteMutex;
	int width = 10;
	ofstream logFile;
	InnerLogger() {
		logFile.open("game.log");
	};
	void writeMessage(string severity, string moduleName, string message) {
		logWriteMutex.lock();
		logFile <<left<< setw(width) << setfill(' ')<< getDateTime() << " | "  <<  setw(width) << setfill(' ') <<moduleName << " | "  << setw(width) << setfill(' ') <<severity << " : " << message << endl;
		logWriteMutex.unlock();
	}

public:
	InnerLogger(InnerLogger const&) = delete;

	void operator=(InnerLogger const&) = delete;

	static InnerLogger& getInstnace() {
		static InnerLogger instance;
		return instance;
	}
	static string getDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%d-%m-%Y | %X", &tstruct);
		return buf;
	}

	void Info(string moduleName, string message) {
		this->writeMessage("INFO", moduleName, message);
	}
	void Error(string moduleName, string message) {
		this->writeMessage("ERROR", moduleName, message);
	}
	void Warning(string moduleName, string message) {
		this->writeMessage("WARNING", moduleName, message);
	}

	~InnerLogger() {
		logFile.flush();
		logFile.close();
	}





};

class Logger {
private:
	string moduleName;
public:
	Logger(string _moduleName) :moduleName(_moduleName) { };
	void Info(string message) {
		InnerLogger::getInstnace().Info(this->moduleName, message);
	}
	void Error( string message) {
		InnerLogger::getInstnace().Error(this->moduleName, message);
	}
	void Warning( string message) {
		InnerLogger::getInstnace().Warning(this->moduleName, message);
	}

};


