#include "Windows.hpp"

#include "Common.hpp"
#include "Window.hpp"
#include "WindowsExceptions.hpp"
#include "GameManager.hpp"
#include "MessageBox.hpp"

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
		TicTacToe::MessageBox::Show(e.what(), "Unhandled exception", TicTacToe::MessageBox::Type::Ok, TicTacToe::MessageBox::Icon::Error);
	}
	exit(0);
}