#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\数独盘.txt";
string folder_path = ".\\players\\"; // 玩家文件夹路径

// 下面是solve页面的一些UI设计参数
const int lowerheight = 50;
const int CELL_SIZE = 60;

// 计算窗口尺寸
const int WINDOW_WIDTH = 1000;  // 增大宽度以容纳按钮
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// 按钮大小
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// 按钮 Y 坐标（由下往上）
const int BUTTON_Y = WINDOW_HEIGHT - BUTTON_HEIGHT - 10;          // 最底下按钮
const int END_BUTTON_Y = BUTTON_Y - BUTTON_HEIGHT - 5;            // “结束”按钮在其上方
const int HINT_BUTTON_Y = END_BUTTON_Y - BUTTON_HEIGHT - 5;       // “提示”按钮在“结束”上方

// 按钮 X 坐标（全部左对齐）
const int EASY_BUTTON_X = 50;
const int MEDIUM_BUTTON_X = 150;
const int HARD_BUTTON_X = 250;
const int GENERATE_BUTTON_X = 350;
const int SOLVE_BUTTON_X = 450;
const int END_BUTTON_X = 550;
const int CLOSE_BUTTON_X = 650;
const int HINT_BUTTON_X = END_BUTTON_X;  // “提示”按钮与“结束”对齐

namespace suduko
{
    int Num = standard;
}
