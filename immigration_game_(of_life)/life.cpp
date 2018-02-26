#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include "board.h"
#include "pixel.h"
#include "resource.h"

#define SPACE 50
#define STARTPIECES 5
#define ADVANCE 10  // 1 used for testing

void drawBoard(const HDC& hdc, const RECT& rect, const HBRUSH& hb0, const HBRUSH& hb1, const HBRUSH& hb2, const Board& life);
string getMessage(const HDC& hdc, const Board& life, const int turn, const int pOnePieces, const int pTwoPieces);
void clearText(const HDC& hdc, const RECT& rect, const TEXTMETRIC& textmet, const string buffer);
void setTurn(const Board& life, int& turn);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "Immigration Game (of Life)";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, "Immigration Game (of Life)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static Board life = Board();
	static unsigned int pOnePieces = STARTPIECES;
	static unsigned int pTwoPieces = STARTPIECES;
	static int turn;
	static string buffer;
	static int background = 0x000000;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	TEXTMETRIC textmet;
	HBRUSH hb0;
	HBRUSH hb1;
	HBRUSH hb2;
	OPENFILENAME file;
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	char szFile[501];
	int width;
	int height;
	int topleftx;
	int bottomrightx;
	int toplefty;
	int bottomrighty;
	int clickx;
	int clicky;
	int realx;
	int realy;
	int dimension = life.getDimension();

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);

		life.setColorOne(0x6666FF);
		life.setColorTwo(0xFF751A);

		srand((unsigned int)time(NULL));

		turn = rand() % 2 + 1;

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		GetTextMetrics(hdc, &textmet);

		hb0 = CreateSolidBrush(background);
		hb1 = CreateSolidBrush(life.getColorOne());
		hb2 = CreateSolidBrush(life.getColorTwo());

		drawBoard(hdc, rect, hb0, hb1, hb2, life);

		clearText(hdc, rect, textmet, buffer);

		buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

		TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));
		
		DeleteObject(hb0);
		DeleteObject(hb1);
		DeleteObject(hb2);

		EndPaint(hwnd, &ps);

		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_HELP:
			MessageBox(hwnd, "The Immigration Game (of Life) is a multiplayer (two player) game based on Conway's Game of Life. The game is played on a twenty-five by twenty-five unit board, and at the start, each player is allowed to place five pieces on the board (the two players' pieces have different colors). The board \"advances\" (without the players doing anything) according to the rules defined by Conway's Game of Life (a cell with less than two \"live\" neighbors or more than three \"live\" neighbors \"dies,\" a cell with two or three \"live\" neighbors comes alive, and a dead cell with exactly three \"live\" neighbors comes alive and takes on the color of the majority of the \"live\" cells). After the players place their five pieces, the board advances ten times; at that time, the player with more pieces on the board is allowed to place another piece on an unoccupied place on the board (if the players have an equal number of pieces, the board will advance until a player has more pieces than the other). Once a player places their piece, the board advances ten more times, and the player with more pieces may place another piece on the board; this is repeated. Once only cells of one color remain on the board, the player with the live cells on the board wins; if no cells remain on the board, the game is a tie.", "Instructions", MB_HELP);

			break;

		case ID_FILE_OPEN:
			ZeroMemory(&file, sizeof(file));

			file.lStructSize = sizeof(file);
			file.hwndOwner = hwnd;
			file.lpstrFile = szFile;
			file.lpstrFile[0] = '\0';
			file.nMaxFile = sizeof(szFile);
			file.lpstrFilter = ".txt\0*.txt\0\0";
			file.lpstrDefExt = "txt";
			file.nFilterIndex = 1;
			file.lpstrFileTitle = NULL;
			file.nMaxFileTitle = 0;
			file.lpstrInitialDir = NULL;
			file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&file) == TRUE)
			{
				ifstream is(file.lpstrFile);

				if (!is)
				{
					cerr << "Error opening input file \"" << file.lpstrFile << ".\"" << endl;

					return -1;
				}

				life.openBoard(is, background, turn, pOnePieces, pTwoPieces);

				is.close();

				if (!is)
				{
					cerr << "Error closing input file \"" << file.lpstrFile << ".\"" << endl;

					return -1;
				}

				// redraw window:

				hdc = GetDC(hwnd);

				GetClientRect(hwnd, &rect);
				GetTextMetrics(hdc, &textmet);

				hb0 = CreateSolidBrush(background);
				hb1 = CreateSolidBrush(life.getColorOne());
				hb2 = CreateSolidBrush(life.getColorTwo());

				drawBoard(hdc, rect, hb0, hb1, hb2, life);

				clearText(hdc, rect, textmet, buffer);

				buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

				TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));

				DeleteObject(hb0);
				DeleteObject(hb1);
				DeleteObject(hb2);

				EndPaint(hwnd, &ps);

				//
			}

			break;

		case ID_FILE_SAVEAS:
			ZeroMemory(&file, sizeof(file));

			file.lStructSize = sizeof(file);
			file.hwndOwner = hwnd;
			file.lpstrFile = szFile;
			file.lpstrFile[0] = '\0';
			file.nMaxFile = sizeof(szFile);
			file.lpstrFilter = ".txt\0*.txt\0\0";
			file.lpstrDefExt = "txt";
			file.nFilterIndex = 1;
			file.lpstrFileTitle = NULL;
			file.nMaxFileTitle = 0;
			file.lpstrInitialDir = NULL;
			file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetSaveFileName(&file) == TRUE)
			{
				ofstream os(file.lpstrFile);

				if (!os)
				{
					cerr << "Error opening output file \"" << file.lpstrFile << ".\"" << endl;

					return -1;
				}

				life.saveBoard(os, background, turn, pOnePieces, pTwoPieces);

				os.close();

				if (!os)
				{
					cerr << "Error closing output file \"" << file.lpstrFile << ".\"" << endl;

					return -1;
				}
			}

			break;

		case ID_PLAYERONE_COLOR:
			ZeroMemory(&cc, sizeof(cc));

			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hwnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = life.getColorOne();
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE)
			{
				life.setColorOne(cc.rgbResult);

				// redraw window:

				hdc = GetDC(hwnd);

				GetClientRect(hwnd, &rect);
				GetTextMetrics(hdc, &textmet);

				hb0 = CreateSolidBrush(background);
				hb1 = CreateSolidBrush(life.getColorOne());
				hb2 = CreateSolidBrush(life.getColorTwo());

				drawBoard(hdc, rect, hb0, hb1, hb2, life);

				clearText(hdc, rect, textmet, buffer);

				buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

				TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));

				DeleteObject(hb0);
				DeleteObject(hb1);
				DeleteObject(hb2);

				EndPaint(hwnd, &ps);

				//
			}

			break;

		case ID_PLAYERTWO_COLOR:
			ZeroMemory(&cc, sizeof(cc));

			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hwnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = life.getColorTwo();
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE)
			{
				life.setColorTwo(cc.rgbResult);

				// redraw window:

				hdc = GetDC(hwnd);

				GetClientRect(hwnd, &rect);
				GetTextMetrics(hdc, &textmet);

				hb0 = CreateSolidBrush(background);
				hb1 = CreateSolidBrush(life.getColorOne());
				hb2 = CreateSolidBrush(life.getColorTwo());

				drawBoard(hdc, rect, hb0, hb1, hb2, life);

				clearText(hdc, rect, textmet, buffer);

				buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

				TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));

				DeleteObject(hb0);
				DeleteObject(hb1);
				DeleteObject(hb2);

				EndPaint(hwnd, &ps);

				//
			}

			break;

		case ID_BACKGROUND_COLOR:
			ZeroMemory(&cc, sizeof(cc));

			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hwnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = background;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE)
			{
				background = cc.rgbResult;

				// redraw window:

				hdc = GetDC(hwnd);

				GetClientRect(hwnd, &rect);
				GetTextMetrics(hdc, &textmet);

				hb0 = CreateSolidBrush(background);
				hb1 = CreateSolidBrush(life.getColorOne());
				hb2 = CreateSolidBrush(life.getColorTwo());

				drawBoard(hdc, rect, hb0, hb1, hb2, life);

				clearText(hdc, rect, textmet, buffer);

				buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

				TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));

				DeleteObject(hb0);
				DeleteObject(hb1);
				DeleteObject(hb2);

				EndPaint(hwnd, &ps);

				//
			}

			break;

		case ID_NEW_GAME:
			hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rect);
			GetTextMetrics(hdc, &textmet);

			srand((unsigned int)time(NULL));

			turn = rand() % 2 + 1;
			pOnePieces = STARTPIECES;
			pTwoPieces = STARTPIECES;
			hb0 = CreateSolidBrush(background);
			hb1 = CreateSolidBrush(life.getColorOne());
			hb2 = CreateSolidBrush(life.getColorTwo());
			width = rect.right / dimension;
			height = (rect.bottom - SPACE) / dimension;
			topleftx = 0;
			toplefty = 0;
			bottomrightx = width;
			bottomrighty = height;

			for (int i = 0; i < dimension; i = i + 1)
			{
				for (int j = 0; j < dimension; j = j + 1)
				{
					if (life[i][j].getColor() != -1)
					{
						life[i][j] = -1;
					}
				}
			}

			for (int i = 0; i < dimension; i = i + 1)
			{
				for (int j = 0; j < dimension; j = j + 1)
				{
					if (life[i][j].getColor() < 0)
					{
						SelectObject(hdc, hb0);
						Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
					}
					else if (life[i][j].getColor() == life.getColorOne())
					{
						SelectObject(hdc, hb1);
						Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
					}
					else
					{
						SelectObject(hdc, hb2);
						Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
					}

					topleftx = topleftx + width;
					bottomrightx = bottomrightx + width;
				}

				topleftx = 0;
				bottomrightx = width;
				toplefty = toplefty + height;
				bottomrighty = bottomrighty + height;
			}

			clearText(hdc, rect, textmet, buffer);

			buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

			TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));

			DeleteObject(hb0);
			DeleteObject(hb1);
			DeleteObject(hb2);

			ReleaseDC(hwnd, hdc);

			break;
		}

		return 0;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

		GetClientRect(hwnd, &rect);
		GetTextMetrics(hdc, &textmet);

		width = rect.right / dimension;
		height = (rect.bottom - SPACE) / dimension;
		clickx = LOWORD(lParam);
		clicky = HIWORD(lParam);
		realx = clickx / width;
		realy = clicky / height;
		hb0 = CreateSolidBrush(background);
		hb1 = CreateSolidBrush(life.getColorOne());
		hb2 = CreateSolidBrush(life.getColorTwo());

		if (clickx / dimension <= width && clicky / dimension <= height)
		{
			if (life[realy][realx].getColor() == -1)
			{
				if (pOnePieces != 0 || pTwoPieces != 0)
				{
					if (turn == 1)
					{
						life[realy][realx] = life.getColorOne();

						pOnePieces = pOnePieces - 1;

						turn = 2;
					}
					else
					{
						life[realy][realx] = life.getColorTwo();

						pTwoPieces = pTwoPieces - 1;

						turn = 1;
					}

					drawBoard(hdc, rect, hb0, hb1, hb2, life);

					if (pOnePieces == 0 && pTwoPieces == 0 && life.gameOver() == false)  // advance life once all pieces are placed
					{
						life.advance(ADVANCE);

						while (life.majorityColor() == -1 && life.gameOver() == false)
						{
							life.advance();
						}

						setTurn(life, turn);

						drawBoard(hdc, rect, hb0, hb1, hb2, life);
					}

					clearText(hdc, rect, textmet, buffer);

					buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

					TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));
				}
				else if (pOnePieces == 0 && pTwoPieces == 0 && life.gameOver() == false)
				{
					if (turn == 1)
					{
						life[realy][realx] = life.getColorOne();
					}
					else
					{
						life[realy][realx] = life.getColorTwo();
					}

					drawBoard(hdc, rect, hb0, hb1, hb2, life);

					life.advance(ADVANCE);

					while (life.majorityColor() == -1 && life.gameOver() == false)
					{
						life.advance();
					}

					setTurn(life, turn);

					drawBoard(hdc, rect, hb0, hb1, hb2, life);

					clearText(hdc, rect, textmet, buffer);

					buffer = getMessage(hdc, life, turn, pOnePieces, pTwoPieces);

					TextOut(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, buffer.c_str(), strlen(buffer.c_str()));
				}
			}
		}

		DeleteObject(hb0);
		DeleteObject(hb1);
		DeleteObject(hb2);

		EndPaint(hwnd, &ps);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void drawBoard(const HDC& hdc, const RECT& rect, const HBRUSH& hb0, const HBRUSH& hb1, const HBRUSH& hb2, const Board& life)
{
	int dimension = life.getDimension();
	int width = rect.right / dimension;
	int height = (rect.bottom - SPACE) / dimension;
	int topleftx = 0;
	int toplefty = 0;
	int bottomrightx = width;
	int bottomrighty = height;

	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			if (life[i][j].getColor() < 0)
			{
				SelectObject(hdc, hb0);
				Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
			}
			else if (life[i][j].getColor() == life.getColorOne())
			{
				SelectObject(hdc, hb1);
				Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
			}
			else
			{
				SelectObject(hdc, hb2);
				Rectangle(hdc, topleftx, toplefty, bottomrightx, bottomrighty);
			}

			topleftx = topleftx + width;
			bottomrightx = bottomrightx + width;
		}

		topleftx = 0;
		bottomrightx = width;
		toplefty = toplefty + height;
		bottomrighty = bottomrighty + height;
	}
}

string getMessage(const HDC& hdc, const Board& life, const int turn, const int pOnePieces, const int pTwoPieces)
{
	string buffer;

	if (turn == 2)
	{
		buffer = "turn: Player " + to_string(turn);

		SetTextColor(hdc, life.getColorTwo());
	}
	else if (turn == 1)
	{
		buffer = "turn: Player " + to_string(turn);

		SetTextColor(hdc, life.getColorOne());
	}
	else if (turn == 0)
	{
		buffer = "Tie game!";

		SetTextColor(hdc, RGB(0, 0, 0));
	}
	else if (turn == -1)
	{
		buffer = "Player " + to_string(-1 * turn) + " wins!";

		SetTextColor(hdc, life.getColorOne());
	}
	else
	{
		buffer = "Player " + to_string(-1 * turn) + " wins!";
		
		SetTextColor(hdc, life.getColorTwo());
	}

	return buffer;
}

void clearText(const HDC& hdc, const RECT& rect, const TEXTMETRIC& textmet, const string buffer)
{
	PatBlt(hdc, rect.right / 2 - strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5, rect.right / 2 + strlen(buffer.c_str()) / 2 * textmet.tmAveCharWidth, rect.bottom - SPACE + 5 + textmet.tmHeight, WHITENESS);
}

void setTurn(const Board& life, int& turn)
{
	if (life.gameOver() == true)
	{
		if (life.majorityColor() == -1)
		{
			turn = 0;
		}
		else if (life.majorityColor() == life.getColorOne())
		{
			turn = -1;
		}
		else
		{
			turn = -2;
		}
	}
	else
	{
		if (life.majorityColor() == life.getColorOne())
		{
			turn = 1;
		}
		else
		{
			turn = 2;
		}
	}
}