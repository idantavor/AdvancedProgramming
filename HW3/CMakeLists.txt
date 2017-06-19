#############################################################
### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###
#############################################################

set(user1 inbarm1)
set(user2 eilon)
set(user3 idantavor)
set(id1 "203123443")
set(id2 "302618616")
set(id3 "304849532")

# All source files needed to compile your exe
set (EXE_SOURCE_FILES
	AlgoDLL.h
	AlgoDLL.cpp
	AlgoStat.h
	AlgoStat.cpp
	BattleManager.h
	BattleManager.cpp
	battleship.h
	battleship.cpp
	BattleThreadPool.h
	BattleThreadPool.cpp
	ConcurentQueue.h
	dirent.h
	GameData.h
	GameData.cpp
	HW3.cpp
	IBattleshipGameAlgo.h
	InflatableBoat.h
	InflatableBoat.cpp
	logger.h
	missileBoat.cpp
	missileBoat.h
	Position.h
	Position.cpp
	SharedBoard.h
	SharedBoard.cpp
	Ship.h
	Ship.cpp
	submarine.h
	submarine.cpp
	TournamentReporter.h
	TournamentReporter.cpp
	UserFleet.h
	UserFleet.cpp
	UserGameData.h
	UserGameData.cpp
	Utility.h
	Utility.cpp
	
)
# All source files needed to compile your smart algorithm's .dll
set (SMART_ALGO_SOURCE_FILES
	Utility.h
	battleship.h
	submarine.h
	missileBoat.h
	InflatableBoat.h
	IBattleshipGameAlgo.h
	BattleShipGameSmartAlgo.h
	BattleShipGameSmartAlgo.cpp
	Position.h
)

##################################
### DO NOT CHANGE FROM HERE ON ###
##################################

message("user: ${user1}, id: ${id1}")

add_definitions(-DUNICODE -D_UNICODE)

set(EX ex3)

# Set exe build parameters
set(EXE_NAME "${EX}.${user1}")
add_executable( ${EXE_NAME}	${EXE_SOURCE_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")

# Set smart algorithm's .dll build parameters
set(ALG3_NAME "${EX}.${user1}.smart")
add_library(${ALG3_NAME} SHARED ${SMART_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG3_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${ALG3_NAME} PROPERTY OUTPUT_NAME "${user1}.smart")
target_compile_definitions(${ALG3_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now