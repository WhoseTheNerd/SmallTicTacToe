#include "checkbox.h"

#include <stdlib.h>

bool checkbox_init(struct checkbox* checkbox, struct checkbox_createinfo* createInfo)
{
	checkbox->title = strdup(createInfo->title);
	checkbox->menuID = createInfo->menuID;

	checkbox->handle = CreateWindow("BUTTON", checkbox->title, WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
		createInfo->x, createInfo->y, createInfo->width, createInfo->height, createInfo->window_handle, (HMENU)createInfo->menuID, createInfo->hInstance, NULL);
	return checkbox->handle != NULL;
}

void checkbox_deinit(struct checkbox* checkbox)
{
	free(checkbox->title);
	DestroyWindow(checkbox->handle);
}

void checkbox_ticked(struct checkbox* checkbox, bool ticked)
{
	CheckDlgButton(checkbox->handle, (int)checkbox->menuID, (UINT)ticked);
}

void checkbox_enabled(struct checkbox* checkbox, bool enabled)
{
	EnableWindow(checkbox->handle, enabled);
}

bool checkbox_checked(struct checkbox* checkbox)
{
	return !IsDlgButtonChecked(checkbox->handle, checkbox->menuID);
}