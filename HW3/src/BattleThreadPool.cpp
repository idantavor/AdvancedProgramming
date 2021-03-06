#include "BattleThreadPool.h"
#include "UserGameData.h"
#include "Utility.h"
#include <ctime>

#include "BattleShipGameSmartAlgo.h"


bool BattleThreadPool::runBattle(AlgoDLL *dllA, AlgoDLL *dllB, GameData *bd)
{
	Logger logger = Logger("runBattleThread");
	logger.Info("running Battle between " + dllA->getName() + " and " + dllB->getName());
	unique_ptr<IBattleshipGameAlgo> playerA, playerB;
	playerA = unique_ptr<IBattleshipGameAlgo>({ dllA->GetAlgoInstance() });
	playerB= unique_ptr<IBattleshipGameAlgo>({ dllB->GetAlgoInstance() });
	GameData gameData;
	bd->clone(gameData);
	UserGameData playerAData(A_TURN,gameData.board);
	UserGameData playerBData(B_TURN, gameData.board);
	playerA->setPlayer(A_TURN);
	playerA->setBoard(playerAData);
	playerB->setPlayer(B_TURN);
	playerB->setBoard(playerBData);
	int currentTurn = A_TURN;// A turn when 0 , B turn when 1;
	bool playerAFin = false; bool playerBFin = false;
	int playerAScore = 0; int playerBScore = 0; 

	while (true) {
		if (playerAFin && playerBFin) {
			dllA->addDraw(playerAScore, playerBScore);
			dllB->addDraw(playerBScore, playerAScore);
			return true;
		}
		auto currAttack = (currentTurn == A_TURN) ? playerA->attack() : playerB->attack();
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
		playerA->notifyOnAttackResult(currentTurn, currAttack, unifiedRes); //notify A
		playerB->notifyOnAttackResult(currentTurn, currAttack, unifiedRes); //notify B

		if (!gameData.fleetA.isNotLose()) { // a lost meaning b won
			dllA->addLose(playerAScore,playerBScore);
			dllB->addWin(playerBScore,playerAScore);
			return true;
		}
		if (!gameData.fleetB.isNotLose()) { // B lost meaning A won
			dllA->addWin(playerAScore,playerBScore);
			dllB->addLose(playerBScore,playerAScore);
			return true;
		}
		if (shouldSwitchTurn) {
			currentTurn = (currentTurn + 1) % 2;
		}
		continue;
	}
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

