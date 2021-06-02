#include "Button.hpp"

#include "WindowsExceptions.hpp"

namespace TicTacToe {

	Button::Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, COLORREF color)
		: m_Handle(nullptr), m_Color(color), m_MenuID(menuID)
	{
		m_Handle = CreateWindow("BUTTON", " ", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, x, y, width, height, hwnd, (HMENU)menuID, hInstance, nullptr);
		if (m_Handle == nullptr) {
			throw WindowsException();
		}
	}

	Button::~Button()
	{
		DestroyWindow(m_Handle);
	}
}