#include "BattleThreadPool.h"
#include "UserGameData.h"
#include "Utility.h"
#include <ctime>

#include "BattleShipGameSmartAlgo.h"

//bool operator==(Coordinate &a, Coordinate&b) {
//	return a.col == b.col && a.depth == b.depth && a.row == b.row;
//}

bool BattleThreadPool::runBattle(AlgoDLL *dllA, AlgoDLL *dllB, GameData *bd)
{
	clock_t stAll = clock();
	unique_ptr<IBattleshipGameAlgo> playerA, playerB;
	playerA = unique_ptr<IBattleshipGameAlgo>({ dllA->GetAlgoInstance() });
	//playerA= unique_ptr<IBattleshipGameAlgo>(new BattleShipGameSmartAlgo());
	playerB= unique_ptr<IBattleshipGameAlgo>({ dllB->GetAlgoInstance() });
	GameData gameData;
	bd->clone(gameData);
	UserGameData playerAData(0,gameData.board);
	UserGameData playerBData(1, gameData.board);
	playerA->setPlayer(0);//change to MACRO
	playerA->setBoard(playerAData);
	playerB->setPlayer(1);
	playerB->setBoard(playerBData);

	int currentTurn = 0;// A turn when 0 , B turn when 1;
	
	bool playerAFin = false; bool playerBFin = false;
	int playerAScore = 0; int playerBScore = 0;
	clock_t clksAattack = 0;
	clock_t clksAnotify = 0;
	clock_t clksBattack = 0;
	clock_t clksBnotify = 0;

	while (true) {
		if (playerAFin && playerBFin) {
			// ask ALON what to do here???? who wins?!
			return true;
		}
		clock_t st = clock();
		auto currAttack = (currentTurn == A_TURN) ? playerA->attack() : playerB->attack();
		if (currentTurn == A_TURN) {
			clksAattack += (clock() - st);
		}
		else {
			clksBattack += (clock() - st);
		}
		if (currAttack == Coordinate(-1, -1,-1)) {
			//mark player finished , switch turns and continue
			(currentTurn == A_TURN) ? playerAFin = true : playerBFin = true;
			currentTurn = (currentTurn + 1) % 2;
			continue;
		}
		Position posToAttack = Position(currAttack.row - 1, currAttack.col - 1,currAttack.depth-1);
		auto result = (currentTurn == A_TURN) ? gameData.fleetB.executeAttack(posToAttack) : gameData.fleetA.executeAttack(posToAttack);
		auto selfResult = (currentTurn == A_TURN) ? gameData.fleetA.executeAttack(posToAttack) : gameData.fleetB.executeAttack(posToAttack);

		AttackResult unifiedRes = AttackResult::Miss;
		bool shouldSwitchTurn = false;
		switch (result.first) {
		case AttackResult::Miss://switch turns 
			shouldSwitchTurn = true;
			break;
		case AttackResult::Hit:
			unifiedRes = AttackResult::Hit;
			break;
		case AttackResult::Sink://update score
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
		st = clock();
		playerA->notifyOnAttackResult(currentTurn, currAttack, unifiedRes); //notify A
		clksAnotify += (clock() - st);
		st = clock();
		playerB->notifyOnAttackResult(currentTurn, currAttack, unifiedRes); //notify B
		clksBnotify += (clock() - st);

		if (!gameData.fleetA.isNotLose()) { // a lost meaning b won
			dllA->addLose();
			dllB->addWin();
			/*return true;*/
			break;
		}
		if (!gameData.fleetB.isNotLose()) { // B lost meaning A won
			dllA->addWin();
			dllB->addLose();
			/*return true;*/
			break;
		}
		if (shouldSwitchTurn) {
			currentTurn = (currentTurn + 1) % 2;
		}
		continue;
	}
	double allTime = double(clock() - stAll) / CLOCKS_PER_SEC;
	double Anotify = double(clksAnotify) / CLOCKS_PER_SEC;
	double Aattack = double(clksAattack) / CLOCKS_PER_SEC;
	double Bnotify = double(clksBnotify) / CLOCKS_PER_SEC;
	double Battack = double(clksBattack) / CLOCKS_PER_SEC;
	cout << "game took " << allTime<<endl;
	cout << "A, notify: " << Anotify << ", attack: " << Aattack << endl;
	cout << "B, notify: " << Bnotify << ", attack: " << Battack << endl;
	return true;
}

void BattleThreadPool::HandleBattleQueue()
{
	tuple<AlgoDLL*, AlgoDLL*, GameData*> gameToRun;
	while (!this->stopThreads) {
		try {
			gameToRun = this->gamesQueue.get();
		}
		catch (exception e) {
			break;
		}
		this->runBattle(std::get<0>(gameToRun), std::get<1>(gameToRun), std::get<2>(gameToRun));
	};
}
;

