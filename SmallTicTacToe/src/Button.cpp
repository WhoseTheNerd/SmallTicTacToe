#include "Button.hpp"

#include "WindowsExceptions.hpp"

namespace TicTacToe {

	Button::Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, COLORREF color)
		: m_Handle(nullptr), m_Color(color), m_MenuID(menuID), m_Title(" ")
	{
		m_Handle = CreateWindow("BUTTON", m_Title.c_str(), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, x, y, width, height, hwnd, (HMENU)menuID, hInstance, nullptr);
		if (m_Handle == nullptr) {
			throw WindowsException();
		}
	}

	Button::Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, const char* title)
		: m_Handle(nullptr), m_Color(RGB(255, 255, 255)), m_MenuID(menuID), m_Title(title)
	{
		m_Handle = CreateWindow("BUTTON", title, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, x, y, width, height, hwnd, (HMENU)menuID, hInstance, nullptr);
		if (m_Handle == nullptr) {
			throw WindowsException();
		}
	}

	Button::~Button()
	{
		DestroyWindow(m_Handle);
	}
	
	void Button::SetTitle(const std::string& title)
	{
		m_Title = title;
		if (!SetWindowText(m_Handle, m_Title.c_str())) {
			throw WindowsException();
		}
	}
	
	void Button::SetState(bool enabled)
	{
		EnableWindow(m_Handle, enabled);
	}
}