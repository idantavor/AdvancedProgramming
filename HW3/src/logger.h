#pragma once
#include <stdio.h>
#include <time.h>
#include <string>
#include <mutex>
#include <fstream>
#include <iostream>

using namespace std;
class Logger {

private:
	mutex logWriteMutex;
	ofstream logFile;
	Logger() {
		logFile.open("game.log");
	};
	void writeMessage(string severity, string messege) {
		logWriteMutex.lock();
		logFile << getDateTime() << " | " << severity << " : " << messege << endl;
		logWriteMutex.unlock();
	}

public:
	Logger(Logger const&) = delete;
	
	void operator=(Logger const&)=delete;
	
	static Logger& getInstnace() {
		static Logger instance;
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
	
	void Info(string message) {
		this->writeMessage("INFO",message);
	}
	void Error(string message) {
		this->writeMessage("ERROR", message);
	}
	void Warning(string message) {
		this->writeMessage("WARNING", message);
	}

	~Logger() {
		logFile.flush();
		logFile.close();
	}





};
