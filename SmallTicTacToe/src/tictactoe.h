#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "button.h"
#include "checkbox.h"
#include "window.h"

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

enum Cell
{
	TCT_CELL_EMPTY = 0,
	TCT_CELL_CROSS = 1,
	TCT_CELL_NOUGHT = -1
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

enum State
{
	TCT_STATE_PLAYER_VS_PLAYER,
	TCT_STATE_PLAYER_VS_BOT,
	TCT_STATE_PLAYER_VS_AI
};

struct WinnerStatus
{
	enum Player player;
	enum WinningMove move;
};

struct GameStuff
{
	enum State state;
	enum Player player;
	bool game_started;
};

struct tile
{
	struct button button;
	enum Cell cell;
};

static struct GameStuff game_state = { 0 };

static struct checkbox Checkboxes[2] = { 0 };

struct button* FindButton(HWND handle);

struct IntPair
{
	int x;
	int y;
};

static struct IntPair TranslationTable[12] = { 0 };

static struct tile board[9] = { 0 };

// The string that appears in the application's title bar.
static const char* szTitle = "TicTacToe";

void tct_init();
void tct_onCreate(HWND hwnd, HINSTANCE hInstance);
void tct_onClick(enum ButtonKind buttonKind, int buttonID, struct window* window);

struct WinnerStatus check_winner(enum State board[3][3]);