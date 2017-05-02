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
	BattleManager.h
	BattleManager.cpp
	battleship.h
	battleship.cpp
	ConsolePrinter.h
	ConsolePrinter.cpp
	hw2.cpp
	IBattleshipGameAlgo.h
	InflatableBoat.h
	InflatableBoat.cpp
	missileBoat.h
	missileBoat.cpp
	Position.cpp
	Position.h
	Ship.cpp
	Ship.h
	submarine.h
	submarine.cpp
	UserFleet.cpp
	UserFleet.h
	dirent.h
	Utility.h
	Utility.cpp
)
# All source files needed to compile your naive algorithm's .dll
set (NAIVE_ALGO_SOURCE_FILES
	IBattleshipGameAlgo.h
	NaiveAlgo.cpp
	NaiveAlgo.h
	Position.cpp
	Position.h
	
)
# All source files needed to compile your read-from-file algorithm's .dll
set (FILE_ALGO_SOURCE_FILES
	IBattleshipGameAlgo.h
	BattleShipGameFromFile.cpp
	BattleShipGameFromFile.h
)
# All source files needed to compile your smart algorithm's .dll
set (SMART_ALGO_SOURCE_FILES
	IBattleshipGameAlgo.h
	BattleShipGameSmartAlgo.cpp
	BattleShipGameSmartAlgo.h
)

##################################
### DO NOT CHANGE FROM HERE ON ###
##################################

message("user: ${user1}, id: ${id1}")

# Set exe build parameters
set(EXE_NAME "ex2.${user1}")
add_executable( ${EXE_NAME}	${EXE_SOURCE_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")

# Set naive algorithm's .dll build parameters
set(ALG1_NAME "ex2.${user1}.naive")
add_library(${ALG1_NAME} SHARED ${NAIVE_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG1_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG1_NAME} PROPERTY OUTPUT_NAME "${user1}.naive")
target_compile_definitions(${ALG1_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now

# Set read-from-file algorithm's .dll build parameters
set(ALG2_NAME "ex2.${user1}.file")
add_library(${ALG2_NAME} SHARED ${FILE_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG2_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG2_NAME} PROPERTY OUTPUT_NAME "${user1}.file")
target_compile_definitions(${ALG2_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now

# Set smart algorithm's .dll build parameters
set(ALG3_NAME "ex2.${user1}.smart")
add_library(${ALG3_NAME} SHARED ${SMART_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG3_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG3_NAME} PROPERTY OUTPUT_NAME "${user1}.smart")
target_compile_definitions(${ALG3_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now