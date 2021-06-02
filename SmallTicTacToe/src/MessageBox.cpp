#include "MessageBox.hpp"

#include <stdexcept>

namespace TicTacToe {

	static MessageBox::Result IDToResult(int id)
	{
		switch (id)
		{
		case IDABORT: return MessageBox::Result::Abort;
		case IDCANCEL: return MessageBox::Result::Cancel;
		case IDCONTINUE: return MessageBox::Result::Continue;
		case IDIGNORE: return MessageBox::Result::Ignore;
		case IDNO: return MessageBox::Result::No;
		case IDOK: return MessageBox::Result::Ok;
		case IDRETRY: return MessageBox::Result::Retry;
		case IDTRYAGAIN: return MessageBox::Result::TryAgain;
		case IDYES: return MessageBox::Result::Yes;
		default: return MessageBox::Result::None;
		}
	}

	static int TypeToID(MessageBox::Type type)
	{
		switch (type)
		{
		case TicTacToe::MessageBox::Type::AbortRetryIgnore: return MB_ABORTRETRYIGNORE;
		case TicTacToe::MessageBox::Type::CancelTryContinue: return MB_CANCELTRYCONTINUE;
		case TicTacToe::MessageBox::Type::Help: return MB_HELP;
		case TicTacToe::MessageBox::Type::Ok: return MB_OK;
		case TicTacToe::MessageBox::Type::OkCancel: return MB_OKCANCEL;
		case TicTacToe::MessageBox::Type::RetryCancel: return MB_RETRYCANCEL;
		case TicTacToe::MessageBox::Type::YesNo: return MB_YESNO;
		case TicTacToe::MessageBox::Type::YesNoCancel: return MB_YESNOCANCEL;
		}

		throw std::runtime_error("End of the switch enumeration");
	}

	static int IconToID(MessageBox::Icon icon)
	{
		switch (icon)
		{
		case TicTacToe::MessageBox::Icon::Exclamation: return MB_ICONEXCLAMATION;
		case TicTacToe::MessageBox::Icon::Warning: return MB_ICONWARNING;
		case TicTacToe::MessageBox::Icon::Information: return MB_ICONINFORMATION;
		case TicTacToe::MessageBox::Icon::Asterisk: return MB_ICONASTERISK;
		case TicTacToe::MessageBox::Icon::Question: return MB_ICONQUESTION;
		case TicTacToe::MessageBox::Icon::Stop: return MB_ICONSTOP;
		case TicTacToe::MessageBox::Icon::Error: return MB_ICONERROR;
		case TicTacToe::MessageBox::Icon::Hand: return MB_ICONHAND;
		}

		throw std::runtime_error("End of the switch enumeration");
	}

	MessageBox::Result MessageBox::Show(const char* message, const char* title, Type type, Icon icon, HWND hwnd)
	{
		int style = TypeToID(type) | IconToID(icon);
		int result = MessageBoxA(hwnd, message, title, style);
		return IDToResult(result);
	}
}