#include <Windows.h>

#include "Common.hpp"
#include "Window.hpp"
#include "WindowsExceptions.hpp"
#include "GameManager.hpp"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try
	{
		TicTacToe::GameManager manager;
		TicTacToe::WinMainParameters parameters = { hInstance, hPrevInstance, lpCmdLine, nCmdShow };
		TicTacToe::Window window("TicTacToe", parameters, manager.Callbacks);
		window.Run();
	}
	catch (std::exception& e) {
		MessageBox(NULL, e.what(), "Unhandled exception", 0);
	}
	exit(0);
}