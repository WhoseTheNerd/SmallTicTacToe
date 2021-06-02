#pragma once

#include <Windows.h>
#include <array>

#include "Common.hpp"
#include "Tile.hpp"

#include <memory>
#include <optional>
#include <functional>

namespace TicTacToe {

	enum class Player : uint8_t
	{
		None,
		Player1, Player2
	};

	enum class WinningMove : uint8_t
	{
		None,
		Horizontal1, Horizontal2, Horizontal3,
		Vertical1, Vertical2, Vertical3,
		Diagonal1, Diagonal2
	};

	class Board
	{
	public:
		using CalculateTurnFn = std::function<std::pair<Cell, std::optional<uint8_t>>(const std::array<Tile*, 9>&)>;
	public:
		Board(HWND hwnd, HINSTANCE hInstance, CalculateTurnFn calculateTurn);

		Tile& operator[](uint8_t index)
		{
			return *m_Tiles[index];
		}

		Tile& operator[](const std::pair<uint8_t, uint8_t>& index)
		{
			return *m_Tiles[index.first * 3 + index.second];
		}

		const Tile& operator[](uint8_t index) const
		{
			return *m_Tiles[index];
		}

		const Tile& operator[](const std::pair<uint8_t, uint8_t>& index) const
		{
			return *m_Tiles[index.first * 3 + index.second];
		}

		std::optional<std::pair<Player, std::pair<WinningMove, std::array<Button*, 3>>>> CheckWinner();

		const std::array<std::unique_ptr<Tile>, 9>& GetTiles() const { return m_Tiles; }

		std::optional<Button*> FindButton(HWND handle) const;

		bool HandleButtons(HWND hwnd, int button);
	private:
		uint8_t CalculateIndex(uint8_t x, uint8_t y);
		std::array<Tile*, 9> GetRawTiles();
		bool HasGameEnded();
	private:
		std::array<std::unique_ptr<Tile>, 9> m_Tiles;
		CalculateTurnFn m_CalculateTurn;
	};
}