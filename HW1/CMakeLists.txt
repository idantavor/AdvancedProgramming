set(user1 inbarm1)
set(user2 eilon)
set(user3 idantavor)
set(id1 "203123443")
set(id2 "302618616")
set(id3 "304849532")
set (SOURCE_FILES
	BattleManager.h
	BattleManager.cpp
	battleship.h
	BattleShipGameFromFile.h
	BattleShipGameFromFile.cpp
	hw1.cpp
	IBattleshipGameAlgo.h
	InflatableBoat.h
	missileBoat.h
	Position.cpp
	Position.h
	Ship.cpp
	Ship.h
	submarine.h
	UserFleet.cpp
	UserFleet.h
	dirent.h
)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	${SOURCE_FILES}
)
#BUILD_BINARY_OUTPUT_DIRECTORY
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")