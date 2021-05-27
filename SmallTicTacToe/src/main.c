#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "tictactoe.h"

// The main window class name.
static const char* szWindowClass = "win32_tictactoe";

bool running = true;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
int randint(int n);

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

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	srand(time(NULL));

	tct_init();

	/* Setting hIcon, hCursor, hIconSm to null instead of loading sth we save 37 bytes of code */
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = NULL;// LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex)) {
		ShowErrorMessage();
		return EXIT_FAILURE;
	}

	HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_VISIBLE| ((WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX), CW_USEDEFAULT, CW_USEDEFAULT, 960, 540, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		ShowErrorMessage();
		return EXIT_FAILURE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0 && running)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

struct WinnerStatus check_winner(enum State board[3][3]);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const char* greeting = "Hello, world!";

	switch (uMsg)
	{
	case WM_CREATE:
	{
		tct_onCreate(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);

		break;
	}
	case WM_COMMAND:
	{
		int buttonID = LOWORD(wParam);

		enum ButtonKind buttonKind = TCT_BUTTON_KIND_NONE;

		if (buttonID == 1) {
			buttonKind = TCT_BUTTON_KIND_PLAY_AGAINST_BOT;
		}
		else if (buttonID == 2) {
			buttonKind = TCT_BUTTON_KIND_IMPOSSIBLE_MODE;
		}
		else if (buttonID > 2 && buttonID < 12) {
			buttonKind = TCT_BUTTON_KIND_TICTACTOE_GRID;
		}
		
		tct_onClick(buttonKind, buttonID, hwnd);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DRAWITEM:
	{
		if (wParam > 2 && wParam < 12) 
		{
			LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

			struct Button button = FindButton(lpDIS->hwndItem);

			SetDCBrushColor(lpDIS->hDC, button.color);

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

			break;
			//return TRUE;
		}
	}
	case WM_QUIT:
		running = false;
		MessageBox(NULL, "WM_QUIT", szTitle, NULL);
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		running = false;
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



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
