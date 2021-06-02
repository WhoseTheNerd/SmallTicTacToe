#include "Window.hpp"

#include "WindowsExceptions.hpp"

namespace TicTacToe {

	Window::Window(const char* title, const WinMainParameters& parameters, const Callbacks& callbacks)
		: m_ClassName(title), m_Running(true), m_Callbacks(callbacks)
	{
		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = &WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = parameters.hInstance;
		wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = NULL;// LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_ClassName.c_str();
		wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex)) {
			throw WindowsException();
		}

		m_Handle = CreateWindowEx(0, m_ClassName.c_str(), title, WS_VISIBLE | ((WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX), CW_USEDEFAULT, CW_USEDEFAULT, 960, 540, NULL, NULL, parameters.hInstance, this);
		if (!m_Handle) {
			throw WindowsException();
		}

		ShowWindow(m_Handle, parameters.nCmdShow);
		if (!UpdateWindow(m_Handle)) {
			throw WindowsException();
		}
	}

	Window::~Window() noexcept
	{
		DestroyWindow(m_Handle);

		PostQuitMessage(0);
		m_Running = false;
	}

	void Window::Run()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0 && m_Running)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Refresh()
	{
		if (!RedrawWindow(m_Handle, nullptr, nullptr, RDW_INVALIDATE)) {
			throw WindowsException();
		}
	}

	LRESULT Window::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			return m_Callbacks.OnCreate(hwnd, wParam, lParam);
		}
		case WM_COMMAND:
		{
			return m_Callbacks.OnCommand(hwnd, wParam, lParam);
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps = { 0 };
			HDC hdc = BeginPaint(hwnd, &ps);

			if (!FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1))) {
				throw WindowsException();
			}

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DRAWITEM:
		{
			return m_Callbacks.OnDrawItem(hwnd, wParam, lParam);
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* window;
		if (uMsg == WM_NCCREATE) {
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			window = static_cast<Window*>(lpcs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}
		else {
			window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		return window->WindowProcedure(hwnd, uMsg, wParam, lParam);
	}
}