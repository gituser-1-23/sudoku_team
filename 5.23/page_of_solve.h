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

// �ⲿȫ�ֱ��������������������壬�����ض���
extern std::chrono::steady_clock::time_point startTime;
extern bool timing;
extern std::chrono::duration<int> elapsedAtStop;
extern TCHAR resultMessage[64];
extern COLORREF resultColor;

#define N1 (suduko::Num)

// �����������̺����֣�֧�ָ����ͼ�ʱ��
inline void drawBoard(const vector<vector<int>>& board, const vector<vector<bool>>& isInitial,
    int selectedRow = -1, int selectedCol = -1,
    bool timing = false, int hintRow = -1, int hintCol = -1)
{
    // ����������
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

    // ��ɫ������ǰѡ�и���
    if (selectedRow != -1 && selectedCol != -1) {
        int x = selectedCol * CELL_SIZE;
        int y = selectedRow * CELL_SIZE;
        setlinecolor(RGB(255, 255, 0));
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // ��ɫ������ʾ����
    if (hintRow != -1 && hintCol != -1) {
        int x = hintCol * CELL_SIZE;
        int y = hintRow * CELL_SIZE;
        setlinecolor(RGB(0, 0, 255));
        setlinestyle(PS_SOLID, 3);
        rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
    }

    // ��߿�
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

    // ��ʱ����ʾ�߼�
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
        _stprintf_s(timeStr, _T("ʱ�䣺%02d:%02d"), minutes, seconds);
        settextcolor(RGB(255, 255, 0));
        settextstyle(20, 0, _T("����"));
        outtextxy(10, 10, timeStr);
    }

    if (_tcslen(resultMessage) > 0) {
        settextstyle(30, 0, _T("����"));
        settextcolor(resultColor);

        int textWidth = textwidth(resultMessage);
        int textHeight = textheight(resultMessage);

        int centerX = N1 * CELL_SIZE / 2 - textWidth / 2;
        int centerY = N1 * CELL_SIZE / 2 - textHeight / 2;

        outtextxy(centerX, centerY, resultMessage);
    }
}

// ���ư�ť
inline void drawButton(int x, int y, int width, int height, const TCHAR* text, bool isHovered)
{
    COLORREF fillColor = isHovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(fillColor);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(RGB(255, 255, 255));
    settextstyle(18, 0, _T("����"));
    outtextxy(x + 5, y + 5, text);
}