#pragma once

#include <Windows.h>
#include <stdbool.h>

struct WinMainParameters
{
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;
};

typedef LRESULT(*window_create_cb)(HWND, WPARAM, LPARAM, struct window*);
typedef LRESULT(*window_command_cb)(HWND, WPARAM, LPARAM, struct window*);
typedef LRESULT(*window_draw_cb)(HWND, WPARAM, LPARAM, struct window*);

struct window
{
	HWND handle;
	char* className;
	char* title;

	window_create_cb create_cb;
	window_command_cb command_cb;
	window_draw_cb draw_cb;
};

bool window_init(struct window* window, const char* title, struct WinMainParameters parameters);
void window_deinit(struct window* window);
void window_run(struct window* window);
bool window_refresh(struct window* window);