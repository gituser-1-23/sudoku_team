#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include "all_params.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

// 外部全局变量声明，仅声明不定义，避免重定义
extern std::chrono::steady_clock::time_point startTime;
extern bool timing;
extern std::chrono::duration<int> elapsedAtStop;
extern TCHAR resultMessage[64];
extern COLORREF resultColor;

#define N1 (suduko::Num)

// 绘制数独棋盘和数字（支持高亮和计时）
inline void drawBoard(const vector<vector<int>>& board, const vector<vector<bool>>& isInitial,
    int selectedRow = -1, int selectedCol = -1,
    bool timing = false, int hintRow = -1, int hintCol = -1)
{
    // 绘制网格线
    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N1; ++j) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            if (i % 3 == 0 || j % 3 == 0) {
                setlinecolor(BLACK);
                setlinestyle(PS_SOLID, 3);
                rectangle(x - 1, y - 1, x + CELL_SIZE + 1, y + CELL_SIZE + 1);
            }

            setlinecolor(RGB(255, 255, 255));
            setlinestyle(PS_SOLID, 1);
            rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);
        }
    }

    // 黄色高亮当前选中格子
    if (selectedRow != -1 && selectedCol != -1) {
        int x = selectedCol * CELL_SIZE;
        int y = selectedRow * CELL_SIZE;
        setlinecolor(RGB(255, 255, 0));
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // 蓝色高亮提示格子
    if (hintRow != -1 && hintCol != -1) {
        int x = hintCol * CELL_SIZE;
        int y = hintRow * CELL_SIZE;
        setlinecolor(RGB(0, 0, 255));
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // 外边框
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

    // 计时器显示逻辑
    int minutes = 0, seconds = 0;
    if (timing) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        minutes = static_cast<int>(elapsed.count() / 60);
        seconds = static_cast<int>(elapsed.count() % 60);
    }
    else if (elapsedAtStop.count() > 0) {
        minutes = static_cast<int>(elapsedAtStop.count() / 60);
        seconds = static_cast<int>(elapsedAtStop.count() % 60);
    }

    if (timing || elapsedAtStop.count() > 0) {
        TCHAR timeStr[32];
        _stprintf_s(timeStr, _T("时间：%02d:%02d"), minutes, seconds);
        settextcolor(RGB(255, 255, 0));
        settextstyle(20, 0, _T("宋体"));
        outtextxy(10, 10, timeStr);
    }

    if (_tcslen(resultMessage) > 0) {
        settextstyle(30, 0, _T("宋体"));
        settextcolor(resultColor);

        int textWidth = textwidth(resultMessage);
        int textHeight = textheight(resultMessage);

        int centerX = N1 * CELL_SIZE / 2 - textWidth / 2;
        int centerY = N1 * CELL_SIZE / 2 - textHeight / 2;

        outtextxy(centerX, centerY, resultMessage);
    }
}

// 绘制按钮
inline void drawButton(int x, int y, int width, int height, const TCHAR* text, bool isHovered)
{
    COLORREF fillColor = isHovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(fillColor);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(RGB(255, 255, 255));
    settextstyle(18, 0, _T("宋体"));
    outtextxy(x + 5, y + 5, text);
}