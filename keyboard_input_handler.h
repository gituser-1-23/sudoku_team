#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"
#include "page_of_solve.h"  // ȷ�� drawBoard �� drawButtons ����
#include "solution_of_Suduko.h"  // isBoardFull, isValidSudoku

// �����ⲿ����
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// ͨ�ü������봦��������UI������ʤ���ж�
inline void handleKeyboardInput(std::vector<std::vector<int>>& board,
    std::vector<std::vector<bool>>& isInitial,
    int& selectedRow, int& selectedCol,
    int offsetX, int offsetY,
    int gridWidth, int gridHeight,
    int mouseX, int mouseY)
{
    if (selectedRow == -1 || selectedCol == -1 || isInitial[selectedRow][selectedCol]) return;

    // ����������
    for (char ch = '1'; ch <= '9'; ++ch)
    {
        if (GetAsyncKeyState(ch) & 0x8000)
        {
            board[selectedRow][selectedCol] = ch - '0';
            cleardevice();
            setorigin(offsetX, offsetY);
            drawBoard(board, isInitial, selectedRow, selectedCol);
            drawButtons(mouseX, mouseY);

            if (isBoardFull(board)) {
                settextstyle(30, 0, _T("����"));
                if (isValidSudoku(board)) {
                    settextcolor(RGB(0, 255, 0));
                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("��ʤ��"));
                }
                else {
                    settextcolor(RGB(255, 0, 0));
                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("ʧ�ܣ�"));
                }
            }

            Sleep(150);  // ��ֹ�ظ�����
            return;
        }
    }

    // ɾ��������
    if ((GetAsyncKeyState(VK_DELETE) & 0x8000) || (GetAsyncKeyState(VK_BACK) & 0x8000))
    {
        board[selectedRow][selectedCol] = 0;
        cleardevice();
        setorigin(offsetX, offsetY);
        drawBoard(board, isInitial, selectedRow, selectedCol);
        drawButtons(mouseX, mouseY);
        Sleep(150);
    }
}