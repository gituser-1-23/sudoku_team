#pragma once
#include <conio.h>
#include <graphics.h>
#include "all_params.h"  // 包含所有参数配置

// 声明外部变量，以确保在其他文件中可以正确引用它们
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// 键盘输入处理函数
inline void handleKeyboardInput(std::vector<std::vector<int>>& board,
    std::vector<std::vector<bool>>& isInitial,
    int& selectedRow, int& selectedCol,
    int offsetX, int offsetY) // 接收偏移量作为参数
{
    if (selectedRow != -1 && selectedCol != -1)  // 确保格子已选中
    {
        if (_kbhit())  // 检查是否有按键
        {
            char key = _getch();  // 获取键盘输入

            // 只有非初始格子才能被修改
            if (!isInitial[selectedRow][selectedCol])
            {
                if (key >= '1' && key <= '9')  // 输入数字1-9
                {
                    board[selectedRow][selectedCol] = key - '0';
                }
                else if (key == '0' || key == ' ')  // 输入0或空格清空
                {
                    board[selectedRow][selectedCol] = 0;
                }

                // 更新显示
                cleardevice();
                setorigin(offsetX, offsetY);
                drawBoard(board, isInitial, selectedRow, selectedCol);  // 重新绘制棋盘
            }
        }
    }
}
