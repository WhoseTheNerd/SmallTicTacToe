#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

#include "tictactoe.h"
#include "window.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
int randint(int n);
void ShowErrorMessage();

static LRESULT on_create(HWND hwnd, WPARAM wParam, LPARAM lParam, struct window* window)
{
	window->handle = hwnd;
	tct_onCreate(window, ((LPCREATESTRUCT)lParam)->hInstance);

	return 0;
}

static LRESULT on_command(HWND hwnd, WPARAM wParam, LPARAM lParam, struct window* window)
{
	int buttonID = LOWORD(wParam);

	tct_onClick(buttonID, window);

	return 0;
}

static LRESULT on_draw(HWND hwnd, WPARAM wParam, LPARAM lParam, struct window* window)
{
	if (wParam > 2 && wParam < 12)
	{
		LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

		struct button* button = FindButton(lpDIS->hwndItem);

		if (button) {
			SetDCBrushColor(lpDIS->hDC, button->color);
		}
		else {
			SetDCBrushColor(lpDIS->hDC, RGB(255, 255, 255));
		}

		SelectObject(lpDIS->hDC, GetStockObject(DC_BRUSH));

		RoundRect(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
			lpDIS->rcItem.right, lpDIS->rcItem.bottom, 5, 5);

		char text[10] = { 0 };
		GetWindowText(lpDIS->hwndItem, text, 9);

		SetBkMode(lpDIS->hDC, TRANSPARENT);
		RECT textRect = lpDIS->rcItem;
		DrawText(lpDIS->hDC, text, -1, &textRect, DT_CALCRECT);
		lpDIS->rcItem.top += (lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2 - (textRect.bottom - textRect.top) / 2;
		DrawText(lpDIS->hDC, text, -1, &lpDIS->rcItem, DT_CENTER | DT_VCENTER);
	}
	
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	struct WinMainParameters parameters = { 0 };
	parameters.hInstance = hInstance;
	parameters.hPrevInstance = hPrevInstance;
	parameters.lpCmdLine = lpCmdLine;
	parameters.nCmdShow = nCmdShow;

	srand(time(0));

	tct_init();

	struct window window = { 0 };
	window.create_cb = on_create;
	window.command_cb = on_command;
	window.draw_cb = on_draw;
	window_init(&window, "SmallTicTacToe", parameters);

	window_run(&window);

	window_deinit(&window);
	
	exit(0);
}

struct WinnerStatus check_winner(enum State board[3][3]);

int randint(int n)
{
	if ((n - 1) == RAND_MAX) {
		return rand();
	}
	else {
		// Supporting larger values for n would requires an even more
		// elaborate implementation that combines multiple calls to rand()
		// assert(n <= RAND_MAX);

		// Chop off all of the values that would cause skew...
		int end = RAND_MAX / n; // truncate skew
		//assert(end > 0);
		end *= n;

		// ... and ignore results from rand() that fall above that limit.
		// (Worst case the loop condition should succeed 50% of the time,
		// so we can expect to bail out of this loop pretty quickly.)
		int r;
		while ((r = rand()) >= end);

		return r % n;
	}
}

void ShowErrorMessage()
{
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0) {
		return;
	}

	LPSTR messageBuffer = NULL;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	MessageBox(NULL, messageBuffer, "Error - WinAPI", NULL);

	LocalFree(messageBuffer);
}