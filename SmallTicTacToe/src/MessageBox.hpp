#pragma once

#include "Windows.hpp"
#undef IGNORE

namespace TicTacToe {

	class MessageBox
	{
	public:
		enum class Result
		{
			None,
			Abort, Cancel, Continue, Ignore, Yes, No, Ok, Retry, TryAgain
		};
		enum class Type
		{
			AbortRetryIgnore,CancelTryContinue, Help, Ok, OkCancel, RetryCancel, YesNo, YesNoCancel
		};
		enum class Icon
		{
			Exclamation, Warning, Information, Asterisk, Question, Stop, Error, Hand
		};
	public:
		static Result Show(const char* message, const char* title, Type type = Type::Ok, Icon icon = Icon::Information, HWND hwnd = nullptr);
	};
}