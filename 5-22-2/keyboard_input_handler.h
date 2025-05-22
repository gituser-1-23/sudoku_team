#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"
#include "page_of_solve.h"   // drawBoard, drawButtons
#include "solution_of_Suduko.h"  // isBoardFull, isValidSudoku

// �ⲿ����������ֻ�����������壩
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;
extern TCHAR resultMessage[64];
extern COLORREF resultColor;

// ͨ�ü������봦��������UI������ʤ���ж�
inline void handleKeyboardInput(
    std::vector<std::vector<int>>& board,
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
            // �������� false, -1, -1 ������չ���ݣ�������ҪҲ��ɾȥ��
            drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
            drawButtons(mouseX, mouseY);

            if (isBoardFull(board)) {
                if (isValidSudoku(board)) {
                    _tcscpy_s(resultMessage, _T("��д��ȷ��"));
                    resultColor = RGB(0, 255, 0);
                }
                else {
                    _tcscpy_s(resultMessage, _T("��д����"));
                    resultColor = RGB(255, 0, 0);
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
        drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
        drawButtons(mouseX, mouseY);
        Sleep(150);
    }
}
