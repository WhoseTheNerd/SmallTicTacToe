#pragma once

#include "Board.hpp"

#include <optional>

namespace TicTacToe {

	class AI
	{
	public:
		std::optional<uint8_t> RandomChoice(Board* board);

		std::optional<uint8_t> BestMove(Board* board);
	private:

	};
}