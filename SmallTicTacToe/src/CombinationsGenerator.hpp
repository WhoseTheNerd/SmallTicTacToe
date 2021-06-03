#pragma once

#include <array>

namespace TicTacToe {

	template <auto Start, auto End, auto Inc, class F>
	constexpr void constexpr_for(F&& f)
	{
		if constexpr (Start < End)
		{
			f(std::integral_constant<decltype(Start), Start>());
			constexpr_for<Start + Inc, End, Inc>(f);
		}
	}

	template <size_t N = 3>
	[[nodiscard]] constexpr std::array<std::array<std::pair<uint8_t, uint8_t>, N>, 8> CalculateCombinations()
	{
		std::array<std::array<std::pair<uint8_t, uint8_t>, N>, 8> value;
		size_t iterator = 0;
		size_t inner_iterator = 0;

		// Add horizontal/row combinations
		constexpr_for<0, N, 1>([&](auto i) {
			inner_iterator = 0;
			constexpr_for<0, N, 1>([&](auto j) {
				value[iterator][inner_iterator++] = { j, i };
			});
			iterator++;
		});

		// Add Vertical/column combinations
		constexpr_for<0, N, 1>([&](auto i) {
			inner_iterator = 0;
			constexpr_for<0, N, 1>([&](auto j) {
				value[iterator][inner_iterator++] = { i, j };
			});
			iterator++;
		});

		inner_iterator = 0;
		constexpr_for<0, N, 1>([&](auto i) {
			value[iterator][inner_iterator++] = { i, i };
		});
		iterator++;

		inner_iterator = 0;
		uint8_t x = N - 1;
		uint8_t y = 0;
		for (size_t i = 0; i < N; ++i) {
			value[iterator][inner_iterator++] = { y, x };
			x--;
			y++;
		}

		return value;
	}

	namespace Testing {
		constexpr std::array<std::array<std::pair<uint8_t, uint8_t>, 3>, 8> combinations = {
			// Horizontal
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 0), std::make_pair(1, 0), std::make_pair(2, 0)},
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1)},
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 2), std::make_pair(1, 2), std::make_pair(2, 2)},
			// Vertical
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2)},
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 2)},
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(2, 0), std::make_pair(2, 1), std::make_pair(2, 2)},
			// Diagonal
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 0), std::make_pair(1, 1), std::make_pair(2, 2)},
			std::array<std::pair<uint8_t, uint8_t>, 3>{std::make_pair(0, 2), std::make_pair(1, 1), std::make_pair(2, 0)}
		};

		constexpr auto calculated_combinations = CalculateCombinations();
		static_assert(combinations == calculated_combinations, "Calculated combinations are wrong for board size of 3 (N = 3)");
	}
}