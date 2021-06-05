#pragma once

#include <Windows.h>
#include <inttypes.h>
#include <stdbool.h>

struct button
{
	HWND handle;
	COLORREF color;
	uint8_t menuID;
	char* title;
};

struct button_createinfo
{
	HWND window_handle;
	HINSTANCE hInstance;
	const char* title;
	COLORREF color;
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint8_t menuID;
	bool ownerDraw;
};

bool button_init(struct button* button, struct button_createinfo* createInfo);
void button_deinit(struct button* button);
void button_enabled(struct button* button, bool enabled);
char* button_gettext(struct button* button);
bool button_settext(struct button* button, const char* title);