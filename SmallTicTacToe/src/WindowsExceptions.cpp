#include "WindowsExceptions.hpp"

#include <Windows.h>

namespace TicTacToe {

	WindowsException::WindowsException()
		: std::runtime_error("Wrong overload")
	{
		DWORD errorMessageID = GetLastError();
		if (errorMessageID == 0) {
			m_Message = "There is no error";
#if _DEBUG
			__debugbreak();
#endif
			return;
		}

		LPSTR messageBuffer = NULL;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		m_Message = std::string(messageBuffer);

		LocalFree(messageBuffer);

#if _DEBUG
		__debugbreak();
#endif
	}

	WindowsException::WindowsException(const char* message)
		: std::runtime_error(message), m_Message(message)
	{
	}

	const char* WindowsException::what() const
	{
		return m_Message.c_str();
	}

	
}