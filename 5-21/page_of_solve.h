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

// �����������̺�����
void drawBoard(const vector<vector<int>>& board, const vector<vector<bool>>& isInitial, int selectedRow = -1, int selectedCol = -1) {
    // ���������ߣ�˫��Ч��������9��9��Χ��
    for (int i = 0; i <= N1; ++i) {
        for (int j = 0; j <= N1; ++j) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            // ���������ߣ�3x3����߽磩
            if (i % 3 == 0 || j % 3 == 0) {
                setlinecolor(BLACK);
                setlinestyle(PS_SOLID, 3);
                rectangle(x - 1, y - 1, x + CELL_SIZE + 1, y + CELL_SIZE + 1);
            }

            // �����ڲ�ϸ�ߣ��������ӱ߽磩
            setlinecolor(RGB(255, 255, 255)); // Ĭ�ϰ�ɫ�߿�
            setlinestyle(PS_SOLID, 1);
            rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);
        }
    }

    // ������ǰѡ�еĸ��ӣ���ɫ�߿�
    if (selectedRow != -1 && selectedCol != -1) {
        int x = selectedCol * CELL_SIZE;
        int y = selectedRow * CELL_SIZE;
        setlinecolor(RGB(255, 255, 0)); // ʹ�� RGB �����ɫ
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // ȷ�������˫����
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 3);
    rectangle(-1, -1, N1 * CELL_SIZE + 1, N1 * CELL_SIZE + 1);
    setlinestyle(PS_SOLID, 1);
    rectangle(0, 0, N1 * CELL_SIZE, N1 * CELL_SIZE);

    // ��������
    settextstyle(30, 0, _T("����"));
    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N1; ++j) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;

            // ��������
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

// ���ư�ť
void drawButton(int x, int y, int width, int height, const TCHAR* text, bool isHovered)
{
    COLORREF fillColor = isHovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(fillColor);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(RGB(255, 255, 255));
    settextstyle(18, 0, _T("����"));
    outtextxy(x + 5, y + 5, text);
}