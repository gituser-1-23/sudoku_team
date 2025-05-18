// solution_of_Suduko.h
#pragma once
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include "all_params.h"

namespace suduko_solver {
    using Grid = std::array<std::array<int, suduko::Num>, suduko::Num>;

    inline bool isValid(const Grid& grid, int row, int col, int num) {
        for (int i = 0; i < suduko::Num; ++i)
            if (grid[row][i] == num || grid[i][col] == num)
                return false;

        int boxStartRow = row - row % 3;
        int boxStartCol = col - col % 3;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (grid[boxStartRow + i][boxStartCol + j] == num)
                    return false;

        return true;
    }

    inline bool solve(Grid& grid) {
        for (int row = 0; row < suduko::Num; ++row) {
            for (int col = 0; col < suduko::Num; ++col) {
                if (grid[row][col] == 0) {
                    for (int num = 1; num <= suduko::Num; ++num) {
                        if (isValid(grid, row, col, num)) {
                            grid[row][col] = num;
                            if (solve(grid)) return true;
                            grid[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    inline bool loadFromFile(const std::string& path, Grid& grid) {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        for (int i = 0; i < suduko::Num; ++i)
            for (int j = 0; j < suduko::Num; ++j)
                file >> grid[i][j];
        return true;
    }
}

// page_of_solve.h
#pragma once
#include <graphics.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include "all_params.h"
#include "Mouse_ui.h"
#include "solution_of_Suduko.h"

inline void drawBoard(const std::vector<std::vector<int>>& board) {
    int N = suduko::Num;
    for (int i = 0; i <= N; ++i) {
        int width = (i % 3 == 0) ? 3 : 1;
        setlinestyle(PS_SOLID, width);
        line(0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
        line(i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT - lowerheight);
    }
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] != 0) {
                TCHAR buf[2];
                _stprintf_s(buf, _T("%d"), board[i][j]);
                outtextxy(j * CELL_SIZE + CELL_SIZE / 2 - 10, i * CELL_SIZE + CELL_SIZE / 2 - 15, buf);
            }
        }
    }
}

inline void drawButton(int x, int y, int width, int height, const TCHAR* text, bool hovered) {
    COLORREF color = hovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(color);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(WHITE);
    settextstyle(18, 0, _T("宋体"));
    outtextxy(x + 5, y + 5, text);
}

inline std::vector<std::vector<int>> readSudokuFromFile(const std::string& filename) {
    std::vector<std::vector<int>> board(suduko::Num, std::vector<int>(suduko::Num, 0));
    std::ifstream file(filename);
    if (!file.is_open()) return {};
    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < suduko::Num) {
        int col = 0;
        for (char c : line) {
            if (c == ' ') continue;
            if (c == '#') board[row][col] = 0;
            else if (isdigit(c)) board[row][col] = c - '0';
            else return {};
            col++;
        }
        if (col != suduko::Num) return {};
        row++;
    }
    return board;
}

inline bool isSafe(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    int N = suduko::Num;
    for (int i = 0; i < N; ++i) if (board[row][i] == num || board[i][col] == num) return false;
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i + startRow][j + startCol] == num)
                return false;
    return true;
}

inline bool solveSudoku(std::vector<std::vector<int>>& board) {
    int N = suduko::Num;
    for (int row = 0; row < N; ++row)
        for (int col = 0; col < N; ++col)
            if (board[row][col] == 0) {
                for (int num = 1; num <= N; ++num)
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) return true;
                        board[row][col] = 0;
                    }
                return false;
            }
    return true;
}

inline int show() {
    auto board = readSudokuFromFile(suduko_file);
    if (board.empty()) return 1;

    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    drawBoard(board);

    MOUSEMSG msg;
    bool running = true;
    while (running) {
        msg = GetMouseMsg();
        bool solveHover = isButtonHovered(msg.x, msg.y, SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT);
        bool closeHover = isButtonHovered(msg.x, msg.y, CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT);

        drawButton(SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT, _T("求解数独"), solveHover);
        drawButton(CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT, _T("关闭程序"), closeHover);

        switch (msg.uMsg) {
        case WM_LBUTTONDOWN:
            if (isButtonClicked(msg.x, msg.y, SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT)) {
                if (solveSudoku(board)) {
                    cleardevice();
                    drawBoard(board);
                }
                else {
                    cleardevice();
                    settextcolor(RED);
                    settextstyle(30, 0, _T("宋体"));
                    outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 15, _T("无解的数独！"));
                }
            }
            else if (isButtonClicked(msg.x, msg.y, CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT)) {
                running = false;
            }
            break;
        case WM_CLOSE:
            running = false;
            break;
        }
    }
    closegraph();
    return 0;
}
