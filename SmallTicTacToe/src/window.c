#include "window.h"

//#include "posix_functions.h"

#include <stdlib.h>

LRESULT WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, struct window* window);

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	struct window* window;
	if (uMsg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		window = (struct window*)lpcs->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
	}
	else {
		window = (struct window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	return WindowProcedure(hwnd, uMsg, wParam, lParam, window);
}

LRESULT WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, struct window* window)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return window->create_cb(hwnd, wParam, lParam, window);
	case WM_COMMAND:
		return window->command_cb(hwnd, wParam, lParam, window);
	case WM_PAINT:
	{
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_DRAWITEM:
		return window->draw_cb(hwnd, wParam, lParam, window);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool window_init(struct window* window, const char* title, struct WinMainParameters parameters)
{
	window->title = strdup(title);
	window->className = strdup(title);

	/* Setting hIcon, hCursor, hIconSm to null instead of loading icons and cursors we save 37 bytes of code */
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = parameters.hInstance;
	wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = NULL;// LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = window->className;
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wcex) == NULL) {
		ShowErrorMessage();
		return false;
	}

	window->handle = CreateWindowEx(0, window->className, window->title, WS_VISIBLE | ((WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX), CW_USEDEFAULT, CW_USEDEFAULT, 960, 540, NULL, NULL, parameters.hInstance, window);
	if (window->handle == NULL) {
		ShowErrorMessage();
		return false;
	}

	ShowWindow(window->handle, parameters.nCmdShow);
	UpdateWindow(window->handle);

	return true;
}

void window_deinit(struct window* window)
{
	free(window->title);
	free(window->className);
	DestroyWindow(window->handle);
}

void window_run(struct window* window)
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool window_refresh(struct window* window)
{
	return RedrawWindow(window->handle, NULL, NULL, RDW_INVALIDATE) != FALSE;
}