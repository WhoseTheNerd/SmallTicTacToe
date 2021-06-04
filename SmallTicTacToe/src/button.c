#include "button.h"

bool button_init(struct button* button, struct button_createinfo* createInfo)
{
	button->color = createInfo->color;	
	button->title = strdup(createInfo->title);
	button->menuID = createInfo->menuID;
	button->handle = CreateWindow("BUTTON", button->title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, 
		createInfo->x, createInfo->y, createInfo->width, createInfo->height, createInfo->window_handle, (HMENU)createInfo->menuID, createInfo->hInstance, NULL);
	
	return button->handle != NULL;
}

void button_deinit(struct button* button)
{
	free(button->title);
	DestroyWindow(button->handle);
}

void button_enabled(struct button* button, bool enabled)
{
	EnableWindow(button->handle, enabled);
}

char* button_gettext(struct button* button)
{
	char* buffer = malloc(512 * sizeof(char));
	int status = GetWindowText(button->handle, buffer, 512);
	if (status == 0) {
		free(buffer);
		buffer = NULL;
	}
	return buffer;
}

bool button_settext(struct button* button, const char* title)
{
	free(button->title);
	button->title = strdup(title);

	return SetWindowText(button->handle, title) != NULL;
}