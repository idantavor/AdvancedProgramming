#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include "Position.h"
#include  "IBattleshipGameAlgo.h"
#define BASE_TEXT_COLOR 0xf
#define SHIP_COLOR_PLAYER_B 0xD
#define SHIP_COLOR_PLAYER_A 0xA
#define ATACK_COLOR 0xC
#define X_OFFSET 4
#define Y_OFFSET 2
#define ATTACK_FLICK_NUM 3
#define ATTACK_FLICK_DELAY 100
#define MISS_FLICK_NUM 1
#define MISS_FLICK_DELAY 200
#define ATTACKS_DELAY 500
using namespace std;
class ConsolePrinter{

private:
	string * board = NULL;
	int board_size = 0;
	void gotoxy(int column, int line);
	int wherex();
	int wherey();
	COORD table_start = { 0,0 };
	void setcursor(bool visible, DWORD size);
	int get_color_by_char(char c);
	void setTextColor(int color) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	};
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int delay = ATTACKS_DELAY;
	void print_attack_animation(Position pos, int color, int delay,int flash_num);
	COORD table_end = { 0,0 };


public:
	ConsolePrinter(string* board, int board_size) {
		this->board = board;
		this->board_size = board_size;
		
	}
	ConsolePrinter() {
		this->board = NULL;
		this->board_size = 0;

	}
	void setBoard(string*board, int board_size) {
		this->board = new string[board_size];
		for (int i = 0; i < board_size; i++) {
			this->board[i] = board[i];
		}
		this->board_size = board_size;
	}
	void print_in_color(int color, string text);

	void print_char( char c);

	void print_attack(Position pos, int turn, AttackResult result);

	void print_borad();
	
	void print_at_location(int x, int y, int color,string print);
	
	void set_delay(int delay) { this->delay = delay; };

	COORD set_font_size(int x, int y);

	void move_cursor_to_end() {
		gotoxy(table_end.X, table_end.Y);
	}
	~ConsolePrinter() {
		if (this->board != NULL) {
			delete[] this->board;
		}
	}
};