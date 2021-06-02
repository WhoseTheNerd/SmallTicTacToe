#pragma once

#include "Window.hpp"
#include "Board.hpp"

#include <memory>

namespace TicTacToe {

	enum class State
	{
		PlayerVsPlayer,
		PlayerVsBot,
		PlayerVsAI
	};

	class GameManager
	{
	public:
		GameManager();

		Window::Callbacks Callbacks;
	private:
		std::unique_ptr<Board> m_Board;
		Board::CalculateTurnFn m_CalculateTurn;

		State m_State = State::PlayerVsPlayer;
		std::array<HWND, 2> m_Checkboxes;
	};
}