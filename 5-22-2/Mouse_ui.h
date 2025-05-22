#pragma once
#include <conio.h>
#include <vector>
#include "all_params.h"

// �ⲿ����������ֻ���������壬ʵ����cppʵ��
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// �ж�������Ƿ��ڰ�ť��
inline bool isButtonClicked(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

// �ж�����Ƿ���ͣ�ڰ�ť��
inline bool isButtonHovered(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return isButtonClicked(x, y, buttonX, buttonY, buttonWidth, buttonHeight);
}

// ���������ѡ����
inline void handleMouseClick(int mouseX, int mouseY)
{
    int row = mouseY / CELL_SIZE;
    int col = mouseX / CELL_SIZE;
    if (row >= 0 && row < standard && col >= 0 && col < standard)
    {
        if (!isInitial[row][col]) {  // ֻ�������ǳ�ʼ��
            selectedRow = row;
            selectedCol = col;
        }
    }
}

// �������봦��������ͼ�β����������ݸ�ֵ��
inline void handleKeyboardInput()
{
    if (selectedRow != -1 && selectedCol != -1)
    {
        if (_kbhit())  // ����Ƿ��м��̰���
        {
            char key = _getch();  // ��ȡ��
            if (key >= '1' && key <= '9')
            {
                board[selectedRow][selectedCol] = key - '0';
            }
            else if (key == '0' || key == ' ')  // ����0��ո����
            {
                board[selectedRow][selectedCol] = 0;
            }
        }
    }
}
