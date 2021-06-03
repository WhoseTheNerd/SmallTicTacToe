#include "Board.hpp"

#include "WindowsExceptions.hpp"

#include "MessageBox.hpp"

#include "CombinationsGenerator.hpp"

#include <algorithm>

namespace TicTacToe {

	static COLORREF DEFAULT_BUTTON_COLOR = RGB(240, 240, 240);

	Board::Board(Window* window, HINSTANCE hInstance, CalculateTurnFn calculateTurn)
		: m_Window(window), m_CalculateTurn(calculateTurn)
	{
		for (uint8_t y = 0; y < 3; ++y) {
			for (uint8_t x = 0; x < 3; ++x) {
				int menuID = (y * 3) + x + 3;
				std::unique_ptr<Button> button = std::make_unique<Button>(window->GetHandle(), menuID, hInstance, x * 100, y * 100 + 50, 100, 100, DEFAULT_BUTTON_COLOR);
				m_Tiles[CalculateIndex(x, y)] = std::make_unique<Tile>(std::move(button));
			}
		}
	}

	void Board::Reset()
	{
		for (std::unique_ptr<Tile>& tile : m_Tiles) {
			tile->cell = Cell::Empty;
			tile->button->SetColor(DEFAULT_BUTTON_COLOR);
			tile->button->SetTitle(" ");
			tile->button->SetState(true);
		}

		m_Window->Refresh();
	}

	std::optional<std::pair<Player, std::array<Button*, 3>>> Board::CheckWinner()
	{
		constexpr auto combinations = CalculateCombinations();

		constexpr std::array<std::pair<Player, Cell>, 2> players = {
			std::make_pair(Player::Player1, Cell::Cross),
			std::make_pair(Player::Player2, Cell::Nought)
		};

		for (std::pair<Player, Cell> playerPair : players) {

			auto [player, cell] = playerPair;

			for (std::array<std::pair<uint8_t, uint8_t>, 3> combination : combinations) {
				std::array<Tile*, 3> tiles;
				std::transform(std::begin(combination), std::end(combination), std::begin(tiles), [this](std::pair<uint8_t, uint8_t> coord) {
					return &(*this)[coord];
				});

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
					
					return std::make_pair(player, buttons);
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

	bool Board::HandleButtons(Window* window, int button)
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

		// Bot choice
		if (coord) {
			Tile* botTile = m_Tiles[coord.value()].get();
			botTile->cell = Cell::Nought;

			botTile->button->SetTitle("O");
		}

		std::string playerText;

		switch (cell)
		{
		case Cell::Empty:
			playerText = " ";
			break;
		case Cell::Cross:
			playerText = "X";
			break;
		case Cell::Nought:
			playerText = "O";
			break;
		default:
			break;
		}

		currentTile->button->SetTitle(playerText);

		std::optional<std::pair<Player, std::array<Button*, 3>>> winner = CheckWinner();

		if (winner) {
			Player player = winner->first;
			std::array<Button*, 3> buttons = winner->second;

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

			for (Button* button : buttons) {
				button->SetColor(color);
			}

			for (const auto& tile : m_Tiles) {
				tile->button->SetState(false);
			}

			window->Refresh();

			MessageBox::Show(message.c_str(), "SmallTicTacToe", MessageBox::Type::Ok, MessageBox::Icon::Information, m_Window->GetHandle());
		}
		else if (HasGameEnded()) {
			for (const auto& tile : m_Tiles) {
				tile->button->SetState(false);
			}
			MessageBox::Show("Game has ended in Tie", "SmallTicTacToe", TicTacToe::MessageBox::Type::Ok, TicTacToe::MessageBox::Icon::Information, m_Window->GetHandle());
			return true;
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