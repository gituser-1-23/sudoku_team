#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"
#include "page_of_solve.h"   // drawBoard, drawButtons
#include "solution_of_Suduko.h"  // isBoardFull, isValidSudoku

// 外部变量声明（只声明，不定义）
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;
extern TCHAR resultMessage[64];
extern COLORREF resultColor;

// 通用键盘输入处理函数，含UI更新与胜负判断
inline void handleKeyboardInput(
    std::vector<std::vector<int>>& board,
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
            // 新增参数 false, -1, -1 保持扩展兼容（如有需要也可删去）
            drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
            drawButtons(mouseX, mouseY);

            if (isBoardFull(board)) {
                if (isValidSudoku(board)) {
                    _tcscpy_s(resultMessage, _T("填写正确！"));
                    resultColor = RGB(0, 255, 0);
                }
                else {
                    _tcscpy_s(resultMessage, _T("填写有误！"));
                    resultColor = RGB(255, 0, 0);
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
        drawBoard(board, isInitial, selectedRow, selectedCol, false, -1, -1);
        drawButtons(mouseX, mouseY);
        Sleep(150);
    }
}
