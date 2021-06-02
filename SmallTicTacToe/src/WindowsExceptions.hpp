#pragma once

#include <stdexcept>
#include <Windows.h>

namespace TicTacToe {

	class WindowsException : public std::runtime_error
	{
	public:
		WindowsException();
		WindowsException(const char* message);

		virtual const char* what() const override;
	private:
		std::string m_Message;
	};
}