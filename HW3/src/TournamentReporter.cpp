#include "TournamentReporter.h"

void TournamentReporter::handleQueue()
{
	while (!this->stopMonitor || !this->statQueue.empty()) {
		if (this->statQueue.empty()) {
			this_thread::sleep_for(1s);
			continue;
		}
		auto stat = statQueue.get();
		auto roundIterator = roundToStatMap.find(stat.totalGames);
		if (roundIterator == roundToStatMap.end()) {
			//case that round doesn't exist in the map
			vector<AlgoStat> newVec;
			newVec.push_back(stat);
			roundToStatMap.insert({ stat.totalGames, newVec });
			roundIterator = roundToStatMap.find(stat.totalGames);
		}
		else {
			//round exists
			(*roundIterator).second.push_back(stat);
		}
		if ((*roundIterator).second.size() == algNum) {
			this->report((*roundIterator).second);
			roundToStatMap.erase(roundIterator);

		}
		continue;
	}
}

void TournamentReporter::report(vector<AlgoStat>& algStatList)
{
	
	sort(algStatList.begin(), algStatList.end(), [](const AlgoStat & first, const AlgoStat & second) {if (first.wins == second.wins)return first.pointsFor > second.pointsFor;else return first.wins > second.wins; });
	this->printHeader();
	for (int  i = 0; i < algStatList.size(); i++) {
		this->printStat(algStatList.at(i), i + 1);
	}
	cout << endl;
}

void TournamentReporter::printHeader() {
	printElement("#", numWidth);
	printElement("Team Name", nameWidth);
	printElement("wins", numWidth);
	printElement("losses", numWidth);
	printElement("%", numWidth);
	printElement("Points For", pointWidth);
	printElement("Points Against", pointWidth);
	cout << endl << endl;
}
void TournamentReporter::printStat(AlgoStat& stat, int number) {
	printElement(std::to_string(number) + '.', numWidth);
	printElement(stat.algName, nameWidth);
	printElement(stat.wins, numWidth);
	printElement(stat.losses, numWidth);
	float percentage = (stat.totalGames == 0) ? 0 : (float(stat.wins) / (stat.wins + stat.losses)) * 100;
	cout << setprecision(2) << fixed;
	printElement(percentage, numWidth);
	printElement(stat.pointsFor, pointWidth);
	printElement(stat.pointsAgainst, pointWidth);
	cout << endl << flush;
}