#pragma once

#include "Window.hpp"
#include "Board.hpp"
#include "AI.hpp"

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
		bool HandleCheckboxes(HWND hwnd, int button);
	private:
		std::unique_ptr<Board> m_Board;
		Board::CalculateTurnFn m_CalculateTurn;

		State m_State = State::PlayerVsPlayer;
		std::array<HWND, 2> m_Checkboxes;
		std::unique_ptr<Button> m_ResetButton;
		Player m_CurrentPlayerTurn = Player::Player1;
		AI m_AI;
		Window* m_Window;
	};
}