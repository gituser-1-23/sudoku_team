#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"
#include "page_of_solve.h"  // 确保 drawBoard 和 drawButtons 可用
#include "solution_of_Suduko.h"  // isBoardFull, isValidSudoku

// 声明外部变量
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// 通用键盘输入处理函数，含UI更新与胜负判断
inline void handleKeyboardInput(std::vector<std::vector<int>>& board,
    std::vector<std::vector<bool>>& isInitial,
    int& selectedRow, int& selectedCol,
    int offsetX, int offsetY,
    int gridWidth, int gridHeight,
    int mouseX, int mouseY)
{
    if (selectedRow == -1 || selectedCol == -1 || isInitial[selectedRow][selectedCol]) return;

    // 数字输入检测
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
                settextstyle(30, 0, _T("宋体"));
                if (isValidSudoku(board)) {
                    settextcolor(RGB(0, 255, 0));
                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("获胜！"));
                }
                else {
                    settextcolor(RGB(255, 0, 0));
                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("失败！"));
                }
            }

            Sleep(150);  // 防止重复输入
            return;
        }
    }

    // 删除键处理
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