#pragma once

#include <Windows.h>

#include <stdbool.h>
#include <inttypes.h>

struct checkbox
{
	HWND handle;
	char* title;
	uint8_t menuID;
};

struct checkbox_createinfo
{
	HWND window_handle;
	HINSTANCE hInstance;
	const char* title;
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint32_t menuID;
};

bool checkbox_init(struct checkbox* checkbox, struct checkbox_createinfo* createInfo);
void checkbox_deinit(struct checkbox* checkbox);
void checkbox_ticked(struct checkbox* checkbox, bool ticked);
void checkbox_enabled(struct checkbox* checkbox, bool enabled);
bool checkbox_checked(struct checkbox* checkbox);