#include "tictactoe.h"

#include <inttypes.h>

void tct_init()
{
	game_state.player = TCT_PLAYER1;
}

void tct_onCreate(HWND hwnd, HINSTANCE hInstance)
{
	Checkboxes[0] = CreateWindow("BUTTON", "Play Against Computer", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 5, 5, 185, 35, hwnd, (HMENU)1, hInstance, NULL);
	CheckDlgButton(hwnd, 1, BST_UNCHECKED);
	Checkboxes[1] = CreateWindow("BUTTON", "Impossible Mode", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 190, 5, 185, 35, hwnd, (HMENU)2, hInstance, NULL);
	CheckDlgButton(hwnd, 2, BST_UNCHECKED);
	EnableWindow(Checkboxes[1], FALSE);

	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			int menuID = (y * 3) + x + 3;
			struct Button button;
			button.handle = CreateWindow("BUTTON", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, x * 100, y * 100 + 50, 100, 100, hwnd, (HMENU)menuID, hInstance, NULL);
			button.color = RGB(240, 240, 240);
			Buttons[x][y] = button;
			struct IntPair pair;
			pair.x = x;
			pair.y = y;
			TranslationTable[menuID] = pair;
		}
	}
}

void tct_onClick(enum ButtonKind buttonKind, int buttonID, HWND hwnd)
{
	switch (buttonKind)
	{
	case TCT_BUTTON_KIND_TICTACTOE_GRID:
	{
		if (game_state.game_started == false) {
			game_state.game_started = true; // Game started
			for (uint8_t i = 0; i < 2; ++i) {
				EnableWindow(Checkboxes[i], FALSE);
			}
		}

		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				char player[3];
				GetWindowText(Buttons[x][y].handle, player, 2);
				if (player[0] == 'X') {
					board[x][y] = TCT_STATE_CROSS;
				}
				else if (player[0] == 'O') {
					board[x][y] = TCT_STATE_NOUGHT;
				}
				else {
					board[x][y] = TCT_STATE_EMPTY;
				}
			}
		}

		char playerChar[2] = { 0 };
		uint8_t x = 0;
		uint8_t y = 0;
		if (game_state.play_against_bot) {
			if (game_state.impossible_mode) {
				// TODO: Implement
			}
			else {
				if (game_state.player == TCT_PLAYER1) {
					playerChar[0] = 'X';
					game_state.player = TCT_PLAYER2;
					struct IntPair pair = TranslationTable[buttonID];
					x = pair.x;
					y = pair.y;
				}
				else if (game_state.player == TCT_PLAYER2) {
					playerChar[0] = 'O';
					game_state.player = TCT_PLAYER1;

					while (1) {
						uint8_t play = (uint8_t)randint(9);
						x = play / 3;
						y = play % 3;
						if (board[x][y] == TCT_STATE_EMPTY) {
							break;
						}
					}
				}
			}
		}
		else {
			struct IntPair pair = TranslationTable[buttonID];
			x = pair.x;
			y = pair.y;
			if (game_state.player == TCT_PLAYER1) {
				playerChar[0] = 'X';
				game_state.player = TCT_PLAYER2;
			}
			else if (game_state.player == TCT_PLAYER2) {
				playerChar[0] = 'O';
				game_state.player = TCT_PLAYER1;
			}
		}

		struct Button button = Buttons[x][y];
		if (!SetWindowText(button.handle, playerChar)) {
			ShowErrorMessage();
			return 0;
		}

		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				char player[3];
				GetWindowText(Buttons[x][y].handle, player, 2);
				if (player[0] == 'X') {
					board[x][y] = TCT_STATE_CROSS;
				}
				else if (player[0] == 'O') {
					board[x][y] = TCT_STATE_NOUGHT;
				}
				else {
					board[x][y] = TCT_STATE_EMPTY;
				}
			}
		}

		struct WinnerStatus winner = check_winner(board);
		if (winner.player != TCT_PLAYER_NONE) {
			for (uint8_t y = 0; y < 3; ++y) {
				for (uint8_t x = 0; x < 3; ++x) {
					EnableWindow(Buttons[x][y].handle, FALSE);
				}
			}
		}

		COLORREF color = { 0 };
		if (winner.player == TCT_PLAYER1) {
			color = RGB(0, 255, 0);
		}
		else if (winner.player == TCT_PLAYER2) {
			color = RGB(255, 0, 0);
		}

		if (winner.move == TCT_WINNING_MOVE_HORIZONTAL_1) {
			Buttons[0][0].color = color;
			Buttons[1][0].color = color;
			Buttons[2][0].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_HORIZONTAL_2) {
			Buttons[0][1].color = color;
			Buttons[1][1].color = color;
			Buttons[2][1].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_HORIZONTAL_3) {
			Buttons[0][2].color = color;
			Buttons[1][2].color = color;
			Buttons[2][2].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_1) {
			Buttons[0][0].color = color;
			Buttons[0][1].color = color;
			Buttons[0][2].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_2) {
			Buttons[1][0].color = color;
			Buttons[1][1].color = color;
			Buttons[1][2].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_3) {
			Buttons[2][0].color = color;
			Buttons[2][1].color = color;
			Buttons[2][2].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_DIAGONAL_1) {
			Buttons[0][0].color = color;
			Buttons[1][1].color = color;
			Buttons[2][2].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
		else if (winner.move == TCT_WINNING_MOVE_DIAGONAL_2) {
			Buttons[0][2].color = color;
			Buttons[1][1].color = color;
			Buttons[2][0].color = color;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}

		if (winner.player == TCT_PLAYER1) {
			MessageBox(hwnd, "Player 1 wins", szTitle, NULL);
		}
		else if (winner.player == TCT_PLAYER2) {
			MessageBox(hwnd, "Player 2 wins", szTitle, NULL);
		}

		if (game_state.play_against_bot && game_state.player == TCT_PLAYER2 && winner.player == TCT_PLAYER_NONE) {
			tct_onClick(TCT_BUTTON_KIND_TICTACTOE_GRID, 3, hwnd);
		}

		break;
	}
	case TCT_BUTTON_KIND_PLAY_AGAINST_BOT:
	case TCT_BUTTON_KIND_IMPOSSIBLE_MODE:
	{

		if (IsDlgButtonChecked(hwnd, buttonID)) {
			CheckDlgButton(hwnd, buttonID, BST_UNCHECKED);

			if (buttonID == 1) {
				game_state.play_against_bot = false;
			}
			else if (buttonID == 2) {
				game_state.impossible_mode = false;
			}
		}
		else {
			CheckDlgButton(hwnd, buttonID, BST_CHECKED);

			if (buttonID == 1) {
				game_state.play_against_bot = true;
			}
			else if (buttonID == 2) {
				game_state.impossible_mode = true;
			}
		}

		if (game_state.play_against_bot) {
			EnableWindow(Checkboxes[1], TRUE);
		}
		else {
			EnableWindow(Checkboxes[1], FALSE);
		}

		break;
	}
	}
}

struct Button FindButton(HWND handle)
{
	for (uint8_t y = 0; y < 3; ++y) {
		for (uint8_t x = 0; x < 3; ++x) {
			struct Button button = Buttons[x][y];
			if (button.handle == handle) {
				return button;
			}
		}
	}
	struct Button result = { 0 };
	return result;
}

struct WinnerStatus check_winner(enum State board[3][3])
{
	struct WinnerStatus result = { 0 };

	enum State state_to_check = { 0 };
	enum Player player = { 0 };

	for (uint8_t i = 0; i < 2; ++i) {

		if (i == 0) {
			state_to_check = TCT_STATE_CROSS;
			player = TCT_PLAYER1;
		}
		else if (i == 1) {
			state_to_check = TCT_STATE_NOUGHT;
			player = TCT_PLAYER2;
		}
		
		if (state_to_check == TCT_STATE_EMPTY) {
			break;
		}

		/* Horizontal */
		if (board[0][0] == state_to_check && board[1][0] == state_to_check && board[2][0] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_1;
			return result;
		}
		if (board[0][1] == state_to_check && board[1][1] == state_to_check && board[2][1] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_2;
			return result;
		}
		if (board[0][2] == state_to_check && board[1][2] == state_to_check && board[2][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_3;
			return result;
		}

		/* Vertical */
		if (board[0][0] == state_to_check && board[0][1] == state_to_check && board[0][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_1;
			return result;
		}
		if (board[1][0] == state_to_check && board[1][1] == state_to_check && board[1][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_2;
			return result;
		}
		if (board[2][0] == state_to_check && board[2][1] == state_to_check && board[2][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_3;
			return result;
		}

		/* Diagonal */
		if (board[0][0] == state_to_check && board[1][1] == state_to_check && board[2][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_DIAGONAL_1;
			return result;
		}
		if (board[0][2] == state_to_check && board[1][1] == state_to_check && board[0][2] == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_DIAGONAL_2;
			return result;
		}
	}

	return result;
}