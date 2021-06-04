#include "tictactoe.h"

#include <inttypes.h>

static struct tile* get_tile(uint8_t x, uint8_t y)
{
	return &board[x * 3 + y];
}

void tct_init()
{
	game_state.player = TCT_PLAYER1;
}

void tct_onCreate(HWND hwnd, HINSTANCE hInstance)
{
	struct checkbox_createinfo createInfo = { 0 };
	createInfo.window_handle = hwnd;
	createInfo.height = hInstance;
	createInfo.title = "Play Against Computer";
	createInfo.x = 5;
	createInfo.y = 5;
	createInfo.width = 185;
	createInfo.height = 35;
	createInfo.menuID = 1;
	if (!checkbox_init(&Checkboxes[0], &createInfo)) {
		ShowErrorMessage();
	}
	createInfo.title = "Impossible Mode";
	createInfo.x = 190;
	createInfo.y = 5;
	createInfo.menuID = 2;
	if (!checkbox_init(&Checkboxes[1], &createInfo)) {
		ShowErrorMessage();
	}

	checkbox_ticked(&Checkboxes[0], false);
	checkbox_ticked(&Checkboxes[1], false);
	checkbox_enabled(&Checkboxes[1], false);

	struct button_createinfo buttonCreateInfo = { 0 };
	buttonCreateInfo.window_handle = hwnd;
	buttonCreateInfo.hInstance = hInstance;
	buttonCreateInfo.title = " ";
	buttonCreateInfo.color = RGB(240, 240, 240);
	buttonCreateInfo.width = 100;
	buttonCreateInfo.height = 100;


	for (uint8_t y = 0; y < 3; ++y) {
		for (uint8_t x = 0; x < 3; ++x) {
			uint8_t menuID = (y * 3) + x + 3;

			struct button button = { 0 };

			buttonCreateInfo.x = x * 100;
			buttonCreateInfo.y = y * 100 + 50;
			buttonCreateInfo.menuID = menuID;

			if (!button_init(&button, &buttonCreateInfo)) {
				ShowErrorMessage();
			}

			get_tile(x, y)->button = button;
		}
	}
}

void tct_onClick(enum ButtonKind buttonKind, int buttonID, struct window* window)
{
	switch (buttonKind)
	{
	case TCT_BUTTON_KIND_TICTACTOE_GRID:
	{
		if (game_state.game_started == false) {
			game_state.game_started = true; // Game started
			for (uint8_t i = 0; i < 2; ++i) {
				checkbox_enabled(&Checkboxes[i], false);
			}
		}

		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				struct button* button = &get_tile(x, y)->button;
				char* player = button_gettext(button);
				if (player == NULL) {
					ShowErrorMessage();
					return;
				}
				if (player[0] == 'X') {
					get_tile(x, y)->cell = TCT_CELL_CROSS;
				}
				else if (player[0] == 'O') {
					get_tile(x, y)->cell = TCT_CELL_NOUGHT;
				}
				else {
					get_tile(x, y)->cell = TCT_CELL_EMPTY;
				}
				free(player);
			}
		}

		char playerChar[2] = { 0 };
		uint8_t x = 0;
		uint8_t y = 0;
		if (game_state.state == TCT_STATE_PLAYER_VS_BOT) {
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
					if (get_tile(x, y)->cell == TCT_CELL_EMPTY) {
						break;
					}
				}
			}
		}
		else if (game_state.state == TCT_STATE_PLAYER_VS_AI) {
			// TODO: Implement minimax
		}
		else {
			int ID = buttonID - 3;
			struct button* button = &board[ID].button;
			x = ID / 3;
			y = ID % 3;
			if (game_state.player == TCT_PLAYER1) {
				playerChar[0] = 'X';
				game_state.player = TCT_PLAYER2;
			}
			else if (game_state.player == TCT_PLAYER2) {
				playerChar[0] = 'O';
				game_state.player = TCT_PLAYER1;
			}
		}

		if (!button_settext(&get_tile(x, y)->button, playerChar)) {
			ShowErrorMessage();
			return;
		}

		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				struct button* button = &get_tile(x, y)->button;
				char* player = button_gettext(button);
				if (player == NULL) {
					ShowErrorMessage();
					return;
				}
				if (player[0] == 'X') {
					get_tile(x,y)->cell = TCT_CELL_CROSS;
				}
				else if (player[0] == 'O') {
					get_tile(x, y)->cell = TCT_CELL_NOUGHT;
				}
				else {
					get_tile(x, y)->cell = TCT_CELL_EMPTY;
				}
				free(player);
			}
		}

		struct WinnerStatus winner = check_winner(board);
		if (winner.player != TCT_PLAYER_NONE) {
			for (uint8_t y = 0; y < 3; ++y) {
				for (uint8_t x = 0; x < 3; ++x) {
					struct button* button = &get_tile(x, y)->button;
					button_enabled(button, false);
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
			get_tile(0, 0)->button.color = color;
			get_tile(1, 0)->button.color = color;
			get_tile(2, 0)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_HORIZONTAL_2) {
			get_tile(0, 1)->button.color = color;
			get_tile(1, 1)->button.color = color;
			get_tile(2, 1)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_HORIZONTAL_3) {
			get_tile(0, 2)->button.color = color;
			get_tile(1, 2)->button.color = color;
			get_tile(2, 2)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_1) {
			get_tile(0, 0)->button.color = color;
			get_tile(0, 1)->button.color = color;
			get_tile(0, 2)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_2) {
			get_tile(1, 0)->button.color = color;
			get_tile(1, 1)->button.color = color;
			get_tile(1, 2)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_VERTICAL_3) {
			get_tile(2, 0)->button.color = color;
			get_tile(2, 1)->button.color = color;
			get_tile(2, 2)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_DIAGONAL_1) {
			get_tile(0, 0)->button.color = color;
			get_tile(1, 1)->button.color = color;
			get_tile(2, 2)->button.color = color;
			window_refresh(window);
		}
		else if (winner.move == TCT_WINNING_MOVE_DIAGONAL_2) {
			get_tile(0, 2)->button.color = color;
			get_tile(1, 1)->button.color = color;
			get_tile(2, 0)->button.color = color;
			window_refresh(window);
		}

		if (winner.player == TCT_PLAYER1) {
			MessageBox(window->handle, "Player 1 wins", szTitle, NULL);
		}
		else if (winner.player == TCT_PLAYER2) {
			MessageBox(window->handle, "Player 2 wins", szTitle, NULL);
		}

		if (game_state.state == TCT_STATE_PLAYER_VS_BOT && game_state.player == TCT_PLAYER2 && winner.player == TCT_PLAYER_NONE) {
			tct_onClick(TCT_BUTTON_KIND_TICTACTOE_GRID, 3, window);
		}

		break;
	}
	case TCT_BUTTON_KIND_PLAY_AGAINST_BOT:
	case TCT_BUTTON_KIND_IMPOSSIBLE_MODE:
	{
		struct checkbox* checkbox = &Checkboxes[buttonID - 1];
		if (checkbox_checked(checkbox)) {
			checkbox_ticked(checkbox, true);

			if (buttonID == 1) {
				game_state.state = TCT_STATE_PLAYER_VS_BOT;
			}
			else if (buttonID == 2) {
				game_state.state = TCT_STATE_PLAYER_VS_AI;
			}
		}
		else {
			checkbox_ticked(checkbox, false);

			if (buttonID == 1) {
				game_state.state = TCT_STATE_PLAYER_VS_PLAYER;
			}
			else if (buttonID == 2) {
				game_state.state = TCT_STATE_PLAYER_VS_BOT;
			}
		}

		if (game_state.state == TCT_STATE_PLAYER_VS_BOT) {
			checkbox_enabled(&Checkboxes[1], true);
		}
		else {
			checkbox_enabled(&Checkboxes[1], false);
		}

		break;
	}
	}
}

struct button* FindButton(HWND handle)
{
	for (uint8_t y = 0; y < 3; ++y) {
		for (uint8_t x = 0; x < 3; ++x) {
			struct button* button = &get_tile(x, y)->button;
			if (button->handle == handle) {
				return button;
			}
		}
	}

	return NULL;
}

struct WinnerStatus check_winner()
{
	struct WinnerStatus result = { 0 };

	enum Cell state_to_check = { 0 };
	enum Player player = { 0 };

	for (uint8_t i = 0; i < 2; ++i) {

		if (i == 0) {
			state_to_check = TCT_CELL_CROSS;
			player = TCT_PLAYER1;
		}
		else if (i == 1) {
			state_to_check = TCT_CELL_NOUGHT;
			player = TCT_PLAYER2;
		}
		
		if (state_to_check == TCT_CELL_EMPTY) {
			break;
		}

		/* Horizontal */
		if (get_tile(0, 0)->cell == state_to_check && get_tile(1, 0)->cell == state_to_check && get_tile(2, 0)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_1;
			return result;
		}
		if (get_tile(0, 1)->cell == state_to_check && get_tile(1, 1)->cell == state_to_check && get_tile(2, 1)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_2;
			return result;
		}
		if (get_tile(0, 2)->cell == state_to_check && get_tile(1, 2)->cell == state_to_check && get_tile(2, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_HORIZONTAL_3;
			return result;
		}

		/* Vertical */
		if (get_tile(0, 0)->cell == state_to_check && get_tile(0, 1)->cell == state_to_check && get_tile(0, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_1;
			return result;
		}
		if (get_tile(1, 0)->cell == state_to_check && get_tile(1, 1)->cell == state_to_check && get_tile(1, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_2;
			return result;
		}
		if (get_tile(2, 0)->cell == state_to_check && get_tile(2, 1)->cell == state_to_check && get_tile(2, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_VERTICAL_3;
			return result;
		}

		/* Diagonal */
		if (get_tile(0, 0)->cell == state_to_check && get_tile(1, 1)->cell == state_to_check && get_tile(2, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_DIAGONAL_1;
			return result;
		}
		if (get_tile(0, 2)->cell == state_to_check && get_tile(1, 1)->cell == state_to_check && get_tile(0, 2)->cell == state_to_check) {
			result.player = player;
			result.move = TCT_WINNING_MOVE_DIAGONAL_2;
			return result;
		}
	}

	return result;
}