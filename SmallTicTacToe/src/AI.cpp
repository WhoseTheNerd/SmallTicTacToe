#include "AI.hpp"

#include "Random.hpp"

namespace TicTacToe {
	
	std::optional<uint8_t> AI::RandomChoice(Board* board)
	{
		int choice = -1;
		while (!board->HasGameEnded()) {
			choice = TicTacToe::Random::Generate(0, 8);
			Tile* tile = board->Get(choice);
			if (tile->cell == Cell::Empty) {
				return static_cast<uint8_t>(choice);
			}
		}
		return {};
	}
	
	std::optional<uint8_t> AI::BestMove(Board* board)
	{
		return std::optional<uint8_t>();
	}
}