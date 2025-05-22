#pragma once
#include <conio.h>
#include <vector>
#include "all_params.h"

// 外部变量声明，只声明不定义，实际由cpp实现
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

// 判断鼠标点击是否在按钮上
inline bool isButtonClicked(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

// 判断鼠标是否悬停在按钮上
inline bool isButtonHovered(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return isButtonClicked(x, y, buttonX, buttonY, buttonWidth, buttonHeight);
}

// 鼠标点击格子选择函数
inline void handleMouseClick(int mouseX, int mouseY)
{
    int row = mouseY / CELL_SIZE;
    int col = mouseX / CELL_SIZE;
    if (row >= 0 && row < standard && col >= 0 && col < standard)
    {
        if (!isInitial[row][col]) {  // 只允许点击非初始格
            selectedRow = row;
            selectedCol = col;
        }
    }
}

// 键盘输入处理函数（无图形操作，仅数据赋值）
inline void handleKeyboardInput()
{
    if (selectedRow != -1 && selectedCol != -1)
    {
        if (_kbhit())  // 检查是否有键盘按下
        {
            char key = _getch();  // 读取键
            if (key >= '1' && key <= '9')
            {
                board[selectedRow][selectedCol] = key - '0';
            }
            else if (key == '0' || key == ' ')  // 输入0或空格清空
            {
                board[selectedRow][selectedCol] = 0;
            }
        }
    }
}
