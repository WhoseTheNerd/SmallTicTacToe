#include "Board.hpp"

#include "WindowsExceptions.hpp"

#include <algorithm>

namespace TicTacToe {

	Board::Board(HWND hwnd, HINSTANCE hInstance, CalculateTurnFn calculateTurn)
		: m_CalculateTurn(calculateTurn)
	{
		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				int menuID = (y * 3) + x + 3;
				std::unique_ptr<Button> button = std::make_unique<Button>(hwnd, menuID, hInstance, x * 100, y * 100 + 50, 100, 100, RGB(240, 240, 240));
				m_Tiles[CalculateIndex(x, y)] = std::make_unique<Tile>(std::move(button));
			}
		}
	}

	std::optional<std::pair<Player, std::pair<WinningMove, std::array<Button*, 3>>>> Board::CheckWinner()
	{
		const std::array<std::array<std::pair<uint8_t, uint8_t>, 3>, 8> combinations = {
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

		const std::array<std::pair<Player, Cell>, 2> players = {
			std::make_pair(Player::Player1, Cell::Cross),
			std::make_pair(Player::Player2, Cell::Nought)
		};

		for (std::pair<Player, Cell> playerPair : players) {

			auto [player, cell] = playerPair;

			for (std::array<std::pair<uint8_t, uint8_t>, 3> combination : combinations) {
				std::array<Tile*, 3> tiles;
				for (uint8_t i = 0; i < combination.size(); ++i) {
					const std::pair<uint8_t, uint8_t> coord = combination[i];
					tiles[i] = &(*this)[coord];
				}

				bool ok = true;
				for (Tile* tile : tiles) {
					if (tile->cell != cell) {
						ok = false;
						break;
					}
				}

				if (ok) {
					std::array<Button*, 3> buttons;
					std::transform(std::begin(tiles), std::end(tiles), std::begin(buttons), [](Tile* tile) {
						return tile->button.get();
					});
					
					return std::make_pair(player, std::make_pair(WinningMove::None, buttons));;
				}
			}
		}

		return {};
	}
	
	std::optional<Button*> Board::FindButton(HWND handle) const
	{
		for (uint8_t i = 0; i < 9; ++i) {
			Button* button = m_Tiles[i]->button.get();
			if (button->GetHandle() == handle) {
				return button;
			}
		}
		return {};
	}

	bool Board::HandleButtons(HWND hwnd, int button)
	{
		if (button < 3 || button > 11) return false;

		Tile* currentTile = m_Tiles[button - 3].get();

		if (currentTile->cell != Cell::Empty) {
			return true;
		}

		auto[cell, coord] = m_CalculateTurn(GetRawTiles());
		if (cell == Cell::Empty) {
			throw std::runtime_error("Wrong cell value from CalculateTurnFn!");
		}

		currentTile->cell = cell;

		char playerText[3] = { 0 };

		switch (cell)
		{
		case Cell::Empty:
			break;
		case Cell::Cross:
			playerText[0] = 'X';
			break;
		case Cell::Nought:
			playerText[0] = 'O';
			break;
		default:
			break;
		}

		if (!SetWindowText(currentTile->button->GetHandle(), playerText)) {
			throw WindowsException();
		}

		std::optional<std::pair<Player, std::pair<WinningMove, std::array<Button*, 3>>>> winner = CheckWinner();

		if (winner) {
			Player player = winner->first;
			std::pair<WinningMove, std::array<Button*, 3>> winningMove = winner->second;

			COLORREF color = RGB(255, 255, 255);
			std::string message;
			switch (player)
			{
			case TicTacToe::Player::None:
				message = "Unknown winner";
				break;
			case TicTacToe::Player::Player1:
				color = RGB(0, 255, 0);
				message = "You win";
				break;
			case TicTacToe::Player::Player2:
				color = RGB(255, 0, 0);
				message = "You lose";
				break;
			}

			std::array<Button*, 3> buttons = winningMove.second;

			for (Button* button : buttons) {
				button->SetColor(color);
			}

			for (const auto& tile : m_Tiles) {
				EnableWindow(tile->button->GetHandle(), FALSE);
			}

			RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE);

			MessageBox(hwnd, message.c_str(), "SmallTicTacToe", 0);
		}
		else if (HasGameEnded()) {
			for (const auto& tile : m_Tiles) {
				EnableWindow(tile->button->GetHandle(), FALSE);
			}
			MessageBox(hwnd, "Game has ended in Tie", "SmallTicTacToe", 0);
			return true;
		}

		if (coord) {
			HandleButtons(hwnd, coord.value() + 3);
		}

		return true;
	}

	uint8_t Board::CalculateIndex(uint8_t x, uint8_t y)
	{
		return y * 3 + x;
	}

	std::array<Tile*, 9> Board::GetRawTiles()
	{
		std::array<Tile*, 9> value;
		std::transform(std::begin(m_Tiles), std::end(m_Tiles), std::begin(value), [](const std::unique_ptr<Tile>& tile) {
			return tile.get();
		});
		return value;
	}
	
	bool Board::HasGameEnded()
	{
		for (const std::unique_ptr<Tile>& tile : m_Tiles) {
			if (tile->cell == Cell::Empty) {
				return false;
			}
		}

		return true;
	}
}