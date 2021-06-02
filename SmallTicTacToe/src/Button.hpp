#pragma once

#include <Windows.h>

namespace TicTacToe {

	class Button
	{
	public:
		Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, COLORREF color);
		Button(const Button&) = delete;
		Button(Button&&) = delete;
		~Button();

		HWND GetHandle() { return m_Handle; }
		COLORREF GetColor() { return m_Color; }

		void SetColor(COLORREF color) { m_Color = color; }
	private:
		HWND m_Handle;
		COLORREF m_Color;
		int m_MenuID;
	};
}