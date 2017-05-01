#include "ConsolePrinter.h"
#include <sstream>
#include "BattleManager.h"
void ConsolePrinter::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

int ConsolePrinter::wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.X; 
}

int ConsolePrinter::wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.Y;
}

void ConsolePrinter::setcursor(bool visible, DWORD size) const
// set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if (size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

int ConsolePrinter::get_color_by_char(char c) const
{
	if (c == ' ')return BASE_TEXT_COLOR;
	if (isupper(c))return SHIP_COLOR_PLAYER_A;
	if (islower(c))return SHIP_COLOR_PLAYER_B;
	return BASE_TEXT_COLOR;
}

void ConsolePrinter::print_attack_animation(Position pos,int color,int delay, int flash_num)
{
	for (int i=0; i < flash_num; i++) {
		string orig_char;
		if (board[pos.getX()][pos.getY()] == 'O' || board[pos.getX()][pos.getY()] == 'o') {
			orig_char = '*';
		}
		else {
			orig_char.push_back(board[pos.getX()][pos.getY()]);
		}
		int orig_col = get_color_by_char(board[pos.getX()][pos.getY()]);
		print_at_location(pos.getY()*2 + X_OFFSET+this->table_start.X,pos.getX()+Y_OFFSET + this->table_start.Y, color, "@");
		Sleep(delay);
		print_at_location(pos.getY()*2 + X_OFFSET + this->table_start.X, pos.getX() + Y_OFFSET + this->table_start.Y, orig_col, orig_char);
		Sleep(delay);
	}
}

void ConsolePrinter::print_in_color(int color, string text)
{
	setTextColor(color);
	cout << text;
	setTextColor(BASE_TEXT_COLOR);
}

void ConsolePrinter::print_char( char c)
{
	int color = get_color_by_char(c);
	setTextColor(color);
	cout << c;
	setTextColor(BASE_TEXT_COLOR);
}

void ConsolePrinter::print_attack(Position pos, int turn, AttackResult result)
{
	setcursor(false, 0);
	int color = (turn == A_TURN) ? SHIP_COLOR_PLAYER_A:SHIP_COLOR_PLAYER_B;
	//for (int i = 0; i < board_size; i++) {
	//	for (int j = 0; j < board_size; j++) {
	//		print_attack_animation(Position(i,j), color);
	//	}
	//}
	
	if (result == AttackResult::Hit || result == AttackResult::Sink) {
		print_attack_animation(pos, color, ATTACK_FLICK_DELAY, ATTACK_FLICK_NUM);
		int color = get_color_by_char(board[pos.getX()][pos.getY()]);
		char hit = islower(board[pos.getX()][pos.getY()]) ? 'o' : 'O';
		board[pos.getX()][pos.getY()] = hit;//mark as hit
		print_at_location(pos.getY() * 2 + X_OFFSET+this->table_start.X, pos.getX() + Y_OFFSET + this->table_start.Y, color, "*");
	}
	else {
		print_attack_animation(pos, color, MISS_FLICK_DELAY, MISS_FLICK_NUM);
	}
	move_cursor_to_end();
	Sleep(delay);
}


void ConsolePrinter::print_borad()
{   
	COORD curr_font_size;
	curr_font_size=set_font_size(0, 25);
	table_start.X = wherex();
	table_start.Y = wherey();
	std::stringstream colum_num_string_stream; string colum_num_string;
	colum_num_string_stream << "    ";
	std::stringstream line_delimiter_stream; string line_delimiter;
	line_delimiter_stream <<"   +";
	
	//build colum number string
	for (int i = 0; i < board_size; i++) {
		colum_num_string_stream << std::to_string(i+1) << " ";
	}
	colum_num_string = colum_num_string_stream.str();

	//build row header/footer
	for (int i = 0; i < board_size; i++) {
		line_delimiter_stream  << line_delimiter  << "--";
	}
	line_delimiter = line_delimiter_stream.str();
	line_delimiter[line_delimiter.length() - 1] = '+';

	cout << colum_num_string << endl;
	cout << line_delimiter << endl;

	for (int i = 0; i < board_size; i++) {
		// print the first character as part of the opener.
		string delim = " |";
		if (i + 1 > 9)delim = "|";
		cout << " " << i+1 <<delim;
		print_char(board[i][0]);
		for (int j = 1; j < board_size; j++) {
			// only add spaces for subsequent characters.
			cout << " ";
			print_char(board[i][j]);
		}
		cout << "|" << endl;
	}
	cout << line_delimiter << endl;
	table_end.X = wherex();
	table_end.Y = wherey();
}

void ConsolePrinter::print_at_location(int x, int y, int color, string print)
{
	setTextColor(color);
	this->gotoxy(x, y);
	cout << print;
	setTextColor(BASE_TEXT_COLOR);
}


COORD ConsolePrinter::set_font_size(int x, int y)
{

	COORD dwFontSize = { x,y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(output, false, &info);
	COORD old_size = info.dwFontSize;
	info.dwFontSize = dwFontSize;
	SetCurrentConsoleFontEx(output, false, &info);
	return old_size;
}

