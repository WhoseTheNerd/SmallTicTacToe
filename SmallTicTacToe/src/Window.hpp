#pragma once

#include "Windows.hpp"
#include <string>
#include <functional>

#include "Common.hpp"

namespace TicTacToe {

	class Window
	{
	public:
		using OnCreateFn = std::function<LRESULT(HWND, WPARAM, LPARAM, Window*)>;
		using OnCommandFn = std::function<LRESULT(HWND, WPARAM, LPARAM)>;
		using OnDrawItemFn = std::function<LRESULT(HWND, WPARAM, LPARAM)>;
		
		struct Callbacks
		{
			OnCreateFn OnCreate;
			OnCommandFn OnCommand;
			OnDrawItemFn OnDrawItem;
		};
	public:
		Window(const char* title, const WinMainParameters& parameters, const Callbacks& callbacks);
		~Window() noexcept;

		void Run();

		void Refresh();

		HWND GetHandle() const { return m_Handle; }
		void SetHandle(HWND hwnd) { m_Handle = hwnd; }
	private:
		LRESULT WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		std::string m_ClassName;
		HWND m_Handle;
		bool m_Running;
		Callbacks m_Callbacks;
	};
}