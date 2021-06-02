#pragma once

#include "Windows.hpp"

#include <string>

namespace TicTacToe {

	class Button
	{
	public:
		Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, COLORREF color);
		Button(HWND hwnd, int menuID, HINSTANCE hInstance, int x, int y, int width, int height, const char* title);
		Button(const Button&) = delete;
		Button(Button&&) = delete;
		~Button();

		HWND GetHandle() { return m_Handle; }
		COLORREF GetColor() { return m_Color; }

		void SetColor(COLORREF color) { m_Color = color; }
		void SetTitle(const std::string& title);

		void SetState(bool enabled);
	private:
		HWND m_Handle;
		COLORREF m_Color;
		int m_MenuID;
		std::string m_Title;
	};
}