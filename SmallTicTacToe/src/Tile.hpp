#pragma once

#include <Windows.h>

#include "Button.hpp"

#include <cinttypes>
#include <memory>

namespace TicTacToe {

	enum class Cell : uint8_t
	{
		Empty,
		Cross,
		Nought
	};

	struct Tile
	{
		std::unique_ptr<Button> button;
		Cell cell;

		Tile(std::unique_ptr<Button> button)
			: button(std::move(button)), cell(Cell::Empty)
		{}
		Tile(std::unique_ptr<Button> button, const Cell& cell)
			: button(std::move(button)), cell(cell)
		{}

		Tile(const Tile&) = delete;
		Tile(Tile&&) = delete;
	};
}