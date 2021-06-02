#include "GameManager.hpp"

#include "Windows.hpp"

#include "WindowsExceptions.hpp"
#include "Random.hpp"

namespace TicTacToe {

	GameManager::GameManager()
	{
		Callbacks.OnCreate = [this](HWND hwnd, WPARAM wParam, LPARAM lParam) -> LRESULT {
			HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

			m_Checkboxes[0] = CreateWindow("BUTTON", "Play Against Computer", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 5, 5, 185, 35, hwnd, (HMENU)1, hInstance, nullptr);
			if (m_Checkboxes[0] == nullptr) {
				throw WindowsException();
			}
			if (!CheckDlgButton(hwnd, 1, BST_UNCHECKED)) {
				throw WindowsException();
			}

			m_Checkboxes[1] = CreateWindow("BUTTON", "Impossible Mode", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 190, 5, 185, 35, hwnd, (HMENU)2, hInstance, nullptr);
			if (m_Checkboxes[1] == nullptr) {
				throw WindowsException();
			}
			if (!CheckDlgButton(hwnd, 2, BST_UNCHECKED)) {
				throw WindowsException();
			}
			EnableWindow(m_Checkboxes[1], FALSE);

			m_ResetButton = std::make_unique<Button>(hwnd, 12, hInstance, 5, 375, 185, 35, "Reset");

			this->m_Board = std::make_unique<Board>(hwnd, hInstance, m_CalculateTurn);
			
			return 0;
		};

		Callbacks.OnCommand = [this](HWND hwnd, WPARAM wParam, LPARAM lParam) -> LRESULT {
			int button = LOWORD(wParam);
			
			if (m_Board->HandleButtons(m_Window, button)) {
				return 0;
			}

			if (HandleCheckboxes(hwnd, button)) {
				return 0;
			}

			if (button == 12) {
				m_Board->Reset();
			}

			return 0;
		};

		Callbacks.OnDrawItem = [this](HWND hwnd, WPARAM wParam, LPARAM lParam) -> LRESULT {
			if (wParam > 2 && wParam < 12)
			{
				LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

				Button* button = m_Board->FindButton(lpDIS->hwndItem).value();

				SetDCBrushColor(lpDIS->hDC, button->GetColor());

				SelectObject(lpDIS->hDC, GetStockObject(DC_BRUSH));

				RoundRect(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
					lpDIS->rcItem.right, lpDIS->rcItem.bottom, 5, 5);

				char text[10] = { 0 };
				GetWindowText(lpDIS->hwndItem, text, 9);

				SetBkMode(lpDIS->hDC, TRANSPARENT);
				RECT textRect = lpDIS->rcItem;
				DrawText(lpDIS->hDC, text, -1, &textRect, DT_CALCRECT);
				lpDIS->rcItem.top += (lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2 - (textRect.bottom - textRect.top) / 2;
				DrawText(lpDIS->hDC, text, -1, &lpDIS->rcItem, DT_CENTER | DT_VCENTER);
			}
			return 0;
		};

		auto P2PFunc = []() -> Cell {
			static bool turn = true;
			if (turn) {
				turn = !turn;
				return Cell::Cross;
			}
			else {
				turn = !turn;
				return Cell::Nought;
			}
		};

		auto PlayerVsBotFunc = [this]() -> std::pair<Cell, std::optional<uint8_t>> {
			std::optional<uint8_t> choice = m_AI.RandomChoice(m_Board.get());

			return std::make_pair(Cell::Cross, choice);
		};

		auto PlayerVsAIFunc = [this]() -> std::pair<Cell, std::optional<uint8_t>> {
			std::optional<uint8_t> choice = m_AI.BestMove(m_Board.get());
			return std::make_pair(Cell::Cross, choice);
		};

		m_CalculateTurn = [=](const std::array<Tile*, 9>& tiles) -> std::pair<Cell, std::optional<uint8_t>> {

			switch (m_State)
			{
			case TicTacToe::State::PlayerVsPlayer:
				return std::make_pair(P2PFunc(), std::optional<uint8_t>());
			case TicTacToe::State::PlayerVsBot:
				return PlayerVsBotFunc();
			case TicTacToe::State::PlayerVsAI:
				return PlayerVsAIFunc();
			}
			
			throw std::runtime_error("Unimplemented state enumeration switch cases!");
		};
	}
	
	bool GameManager::HandleCheckboxes(HWND hwnd, int button)
	{
		if (button < 0 || button > 2) return false;

		if (IsDlgButtonChecked(hwnd, button)) {
			CheckDlgButton(hwnd, button, BST_UNCHECKED);

			if (button == 1) {
				m_State = State::PlayerVsPlayer;
			}
			else if (button == 2) {
				m_State = State::PlayerVsBot;
			}
		}
		else {
			CheckDlgButton(hwnd, button, BST_CHECKED);

			if (button == 1) {
				m_State = State::PlayerVsBot;
			}
			else if (button == 2) {
				m_State = State::PlayerVsAI;
			}
		}

		if (m_State == State::PlayerVsBot || m_State == State::PlayerVsAI) {
			EnableWindow(m_Checkboxes[1], TRUE);
		}
		else {
			EnableWindow(m_Checkboxes[1], FALSE);
			if (!CheckDlgButton(hwnd, 2, BST_UNCHECKED)) {
				throw WindowsException();
			}
		}

		return true;
	}
}