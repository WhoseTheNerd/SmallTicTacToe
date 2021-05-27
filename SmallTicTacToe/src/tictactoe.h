#pragma once

#include <stdbool.h>
#include <Windows.h>

enum ButtonKind
{
	TCT_BUTTON_KIND_NONE,
	TCT_BUTTON_KIND_TICTACTOE_GRID,
	TCT_BUTTON_KIND_PLAY_AGAINST_BOT,
	TCT_BUTTON_KIND_IMPOSSIBLE_MODE
};

enum Player
{
	TCT_PLAYER_NONE,
	TCT_PLAYER1,
	TCT_PLAYER2
};

enum State
{
	TCT_STATE_EMPTY = 0,
	TCT_STATE_CROSS = 1,
	TCT_STATE_NOUGHT = -1
};

enum WinningMove
{
	TCT_WINNING_MOVE_NONE,
	TCT_WINNING_MOVE_HORIZONTAL_1,
	TCT_WINNING_MOVE_HORIZONTAL_2,
	TCT_WINNING_MOVE_HORIZONTAL_3,
	TCT_WINNING_MOVE_VERTICAL_1,
	TCT_WINNING_MOVE_VERTICAL_2,
	TCT_WINNING_MOVE_VERTICAL_3,
	TCT_WINNING_MOVE_DIAGONAL_1,
	TCT_WINNING_MOVE_DIAGONAL_2
};

struct WinnerStatus
{
	enum Player player;
	enum WinningMove move;
};

struct GameStuff
{
	bool play_against_bot;
	bool impossible_mode;
	enum Player player;
	bool game_started;
};

struct GameStuff game_state = { 0 };

HWND Checkboxes[2] = { 0 };

struct Button
{
	HWND handle;
	COLORREF color;
};

struct Button Buttons[3][3] = { 0 };

struct Button FindButton(HWND handle);

struct IntPair
{
	int x;
	int y;
};

struct IntPair TranslationTable[12] = { 0 };

enum State board[3][3] = { 0 };

// The string that appears in the application's title bar.
const char* szTitle = "TicTacToe";

void tct_init();
void tct_onCreate(HWND hwnd, HINSTANCE hInstance);
void tct_onClick(enum ButtonKind buttonKind, int buttonID, HWND hwnd);

struct WinnerStatus check_winner(enum State board[3][3]);