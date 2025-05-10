#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include"all_params.h"
using namespace std;


int N1 = suduko::Num;
// 绘制数独棋盘和数字
void drawBoard(const vector<vector<int>> & board)
{
    // 绘制网格线
    for (int i = 0; i <= N1; ++i) {
        int lineWidth = (i % 3 == 0) ? 3 : 1;
        setlinestyle(PS_SOLID, lineWidth);
        line(0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
        line(i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT - lowerheight);
    }

    // 绘制数字
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N1; ++j) {
            if (board[i][j] != 0) {
                TCHAR num[2];
                _stprintf_s(num, _T("%d"), board[i][j]);
                int x = j * CELL_SIZE + CELL_SIZE / 2 - 10;
                int y = i * CELL_SIZE + CELL_SIZE / 2 - 15;
                outtextxy(x, y, num);
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
    settextcolor(WHITE);
    settextstyle(18, 0, _T("宋体"));
    outtextxy(x + 5, y + 5, text);
}
