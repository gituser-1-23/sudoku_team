#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include "all_params.h"
using namespace std;

int N1 = suduko::Num;

// 绘制数独棋盘和数字
void drawBoard(const vector<vector<int>>& board, const vector<vector<bool>>& isInitial, int selectedRow = -1, int selectedCol = -1) {
    // 绘制网格线（双线效果，仅限9×9格范围）
    for (int i = 0; i <= N1; ++i) {
        for (int j = 0; j <= N1; ++j) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            // 绘制外层粗线（3x3宫格边界）
            if (i % 3 == 0 || j % 3 == 0) {
                setlinecolor(BLACK);
                setlinestyle(PS_SOLID, 3);
                rectangle(x - 1, y - 1, x + CELL_SIZE + 1, y + CELL_SIZE + 1);
            }

            // 绘制内层细线（单个格子边界）
            setlinecolor(RGB(255, 255, 255)); // 默认白色边框
            setlinestyle(PS_SOLID, 1);
            rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);
        }
    }

    // 高亮当前选中的格子（黄色边框）
    if (selectedRow != -1 && selectedCol != -1) {
        int x = selectedCol * CELL_SIZE;
        int y = selectedRow * CELL_SIZE;
        setlinecolor(RGB(255, 255, 0)); // 使用 RGB 定义黄色
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // 确保最外层双粗线
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 3);
    rectangle(-1, -1, N1 * CELL_SIZE + 1, N1 * CELL_SIZE + 1);
    setlinestyle(PS_SOLID, 1);
    rectangle(0, 0, N1 * CELL_SIZE, N1 * CELL_SIZE);

    // 绘制数字
    settextstyle(30, 0, _T("宋体"));
    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N1; ++j) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            // 绘制数字
            if (board[i][j] != 0) {
                TCHAR num[2];
                _stprintf_s(num, _T("%d"), board[i][j]);
                int textX = x + CELL_SIZE / 2 - 10;
                int textY = y + CELL_SIZE / 2 - 15;
                settextcolor(RGB(255, 255, 255));
                outtextxy(textX, textY, num);
            }
        }
    }
}

// 绘制按钮
void drawButton(int x, int y, int width, int height, const TCHAR* text, bool isHovered)
{
    COLORREF fillColor = isHovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(fillColor);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(RGB(255, 255, 255));
    settextstyle(18, 0, _T("宋体"));
    outtextxy(x + 5, y + 5, text);
}