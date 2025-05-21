#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"  // �������в�������

// �����ⲿ��������ȷ���������ļ��п�����ȷ��������
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// �������봦����
inline void handleKeyboardInput(std::vector<std::vector<int>>& board,
    std::vector<std::vector<bool>>& isInitial,
    int& selectedRow, int& selectedCol,
    int offsetX, int offsetY) // ����ƫ������Ϊ����
{
    if (selectedRow != -1 && selectedCol != -1)  // ȷ��������ѡ��
    {
        if (_kbhit())  // ����Ƿ��а���
        {
            char key = _getch();  // ��ȡ��������

            // ֻ�зǳ�ʼ���Ӳ��ܱ��޸�
            if (!isInitial[selectedRow][selectedCol])
            {
                if (key >= '1' && key <= '9')  // ��������1-9
                {
                    board[selectedRow][selectedCol] = key - '0';
                }
                else if (key == '0' || key == ' ')  // ����0��ո����
                {
                    board[selectedRow][selectedCol] = 0;
                }

                // ������ʾ
                cleardevice();
                setorigin(offsetX, offsetY);
                drawBoard(board, isInitial, selectedRow, selectedCol);  // ���»�������
            }
        }
    }
}
