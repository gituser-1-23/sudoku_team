#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "all_params.h"
#include "Mouse_ui.h"
#include "page_of_solve.h"
using namespace std;

extern int selectedRow, selectedCol; // 当前被点击的格子位置
extern string filename;
extern int N;
extern vector<vector<int>> board;
extern vector<vector<bool>> isInitial;
extern int difficulty;
extern int blankCount;

// 检查在 (row, col) 位置放置数字 num 是否合法
bool isSafe(vector<vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) return false;
    }
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) return false;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) return false;
        }
    }
    return true;
}

// 回溯算法求解数独
bool solveSudoku(vector<vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= N; ++num) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 检查当前棋盘是否为有效解
bool isValidSudoku(vector<vector<int>>& board) {
    for (int i = 0; i < N; i++) {
        vector<bool> rowCheck(N + 1, false), colCheck(N + 1, false), boxCheck(N + 1, false);
        for (int j = 0; j < N; j++) {
            // 检查行
            if (board[i][j] < 1 || board[i][j] > 9 || rowCheck[board[i][j]]) return false;
            rowCheck[board[i][j]] = true;
            // 检查列
            if (board[j][i] < 1 || board[j][i] > 9 || colCheck[board[j][i]]) return false;
            colCheck[board[j][i]] = true;
            // 检查3x3宫格
            int row = (i / 3) * 3 + j / 3;
            int col = (i % 3) * 3 + j % 3;
            int num = board[row][col];
            if (num < 1 || num > 9 || boxCheck[num]) return false;
            boxCheck[num] = true;
        }
    }
    return true;
}

// 生成完整数独棋盘
bool generateSudoku(vector<vector<int>>& board, int row = 0, int col = 0) {
    if (row == N) return true;
    if (col == N) return generateSudoku(board, row + 1, 0);
    if (board[row][col] != 0) return generateSudoku(board, row, col + 1);

    vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (size_t i = nums.size() - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        std::swap(nums[i], nums[j]);
    }

    for (int num : nums) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (generateSudoku(board, row, col + 1)) return true;
            board[row][col] = 0;
        }
    }
    return false;
}

// 移除指定数量的数字以生成不同难度并记录初始状态
void removeNumbers(vector<vector<int>>& board, int count) {
    int removed = 0;
    isInitial.assign(N, vector<bool>(N, true)); // 初始全为非空白
    while (removed < count) {
        int row = rand() % N;
        int col = rand() % N;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            isInitial[row][col] = false; // 标记为空白格子
            removed++;
        }
    }
}

// 从文件中读取数独棋盘，处理 # 表示的缺项
vector<vector<int>> readSudokuFromFile(const string& filename) {
    vector<vector<int>> board(N, vector<int>(N, 0));
    isInitial.assign(N, vector<bool>(N, false)); // 默认全为空白格子
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        size_t row = 0;
        while (getline(file, line) && row < static_cast<size_t>(N)) {
            size_t col = 0;
            for (char c : line) {
                if (c == ' ') continue;
                if (c == '#') {
                    board[row][col] = 0;
                    isInitial[row][col] = false;
                }
                else if (isdigit(c)) {
                    board[row][col] = c - '0';
                    isInitial[row][col] = true;
                }
                else {
                    cerr << "Invalid character in file: " << c << endl;
                    isInitial.assign(N, vector<bool>(N, false));
                    return board;
                }
                col++;
            }
            if (col != static_cast<size_t>(N)) {
                cerr << "Invalid line length in file: " << line << endl;
                isInitial.assign(N, vector<bool>(N, false));
                return board;
            }
            row++;
        }
        if (row != static_cast<size_t>(N)) {
            cerr << "Invalid number of lines in file." << endl;
            isInitial.assign(N, vector<bool>(N, false));
            return board;
        }
        file.close();
    }
    return board;
}

// 绘制难度选择按钮和求解按钮
void drawButtons(int mouseX, int mouseY) {
    bool easyHovered = isButtonHovered(mouseX, mouseY, EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool mediumHovered = isButtonHovered(mouseX, mouseY, MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool hardHovered = isButtonHovered(mouseX, mouseY, HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool generateHovered = isButtonHovered(mouseX, mouseY, GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool solveHovered = isButtonHovered(mouseX, mouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool closeHovered = isButtonHovered(mouseX, mouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool endHovered = isButtonHovered(mouseX, mouseY, END_BUTTON_X, END_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool hintHovered = isButtonHovered(mouseX, mouseY, HINT_BUTTON_X, HINT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);

    drawButton(HINT_BUTTON_X, HINT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("提示"), hintHovered);
    drawButton(END_BUTTON_X, END_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("结束"), endHovered);
    drawButton(EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("简单"), easyHovered);
    drawButton(MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("中等"), mediumHovered);
    drawButton(HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("困难"), hardHovered);
    drawButton(GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("开始"), generateHovered);
    drawButton(SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("求解"), solveHovered);
    drawButton(CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("关闭"), closeHovered);
}

// 检查棋盘是否填满
bool isBoardFull(vector<vector<int>>& board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) return false;
        }
    }
    return true;
}


int show()
{
    srand((unsigned)time(0));
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(BLACK); // 设置背景为黑色
    cleardevice();     // 清除屏幕
    board = readSudokuFromFile(filename);

    int gridWidth = N * CELL_SIZE;
    int gridHeight = N * CELL_SIZE;
    int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - gridHeight - lowerheight) / 2;

    setorigin(offsetX, offsetY);

    selectedRow = -1;
    selectedCol = -1;
    drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);


    MOUSEMSG msg;
    bool flag = true;
    int lastMouseX = 0, lastMouseY = 0;

    while (flag) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            lastMouseX = msg.x - offsetX;
            lastMouseY = msg.y - offsetY;

            switch (msg.uMsg) {
            case WM_MOUSEMOVE:
                drawButtons(lastMouseX, lastMouseY);
                break;

            case WM_LBUTTONDOWN:
                if (isButtonClicked(lastMouseX, lastMouseY, EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 0; blankCount = 35;
                    selectedRow = selectedCol = -1;
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 1; blankCount = 45;
                    selectedRow = selectedCol = -1;
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 2; blankCount = 55;
                    selectedRow = selectedCol = -1;
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    board.assign(N, vector<int>(N, 0));
                    generateSudoku(board);
                    removeNumbers(board, blankCount);
                    cleardevice();
                    setorigin(offsetX, offsetY);
                    selectedRow = selectedCol = -1;
                    drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);

                }
                else if (isButtonClicked(lastMouseX, lastMouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    if (solveSudoku(board)) {
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        selectedRow = selectedCol = -1;
                        drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);

                    }
                    else {
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        settextcolor(RGB(255, 0, 0));
                        settextstyle(30, 0, _T("宋体"));
                        outtextxy(gridWidth / 2 - 100, gridHeight / 2 - 15, _T("无解的数独！"));
                    }
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    flag = false;
                    closegraph();
                }
                else {
                    int row = (msg.y - offsetY) / CELL_SIZE;
                    int col = (msg.x - offsetX) / CELL_SIZE;
                    if (row >= 0 && row < N && col >= 0 && col < N) {
                        if (!isInitial[row][col]) {
                            selectedRow = row;
                            selectedCol = col;
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
                            drawButtons(lastMouseX, lastMouseY);
                        }
                    }
                    else {
                        selectedRow = selectedCol = -1;
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
                        drawButtons(lastMouseX, lastMouseY);
                    }
                }
                break;

            case WM_CLOSE:
                flag = false;
                closegraph();
                break;
            }
        }

        // 键盘输入独立处理（不依赖 MOUSEMSG）
        if (selectedRow != -1 && selectedCol != -1 && !isInitial[selectedRow][selectedCol]) {
            for (char ch = '1'; ch <= '9'; ++ch) {
                if (GetAsyncKeyState(ch) & 0x8000) {
                    board[selectedRow][selectedCol] = ch - '0';
                    cleardevice();
                    setorigin(offsetX, offsetY);
                    drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
                    drawButtons(lastMouseX, lastMouseY);

                    if (isBoardFull(board)) {
                        settextstyle(30, 0, _T("宋体"));
                        if (isValidSudoku(board)) {
                            settextcolor(RGB(0, 255, 0));
                            outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("获胜"));
                        }
                        else {
                            settextcolor(RGB(255, 0, 0));
                            outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("失败"));
                        }
                    }

                    Sleep(150); // 防止输入过快
                    break;
                }
            }

            if ((GetAsyncKeyState(VK_BACK) & 0x8000) || (GetAsyncKeyState(VK_DELETE) & 0x8000)) {
                board[selectedRow][selectedCol] = 0;
                cleardevice();
                setorigin(offsetX, offsetY);
                drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
                drawButtons(lastMouseX, lastMouseY);
                Sleep(150);
            }
        }
    }

    return 0;
}
