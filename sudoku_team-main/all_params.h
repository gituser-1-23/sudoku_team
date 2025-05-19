#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\数独盘.txt";
// 下面是solve页面的一些ui设计参数
const int lowerheight = 50;

const int CELL_SIZE = 60;
const int WINDOW_WIDTH = 700; // 增大宽度以容纳所有按钮和棋盘
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// 按钮大小和位置（重新调整以适应新窗口宽度）
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// 按钮位置（6个按钮：简单、中等、困难、生成、求解、关闭）
const int EASY_BUTTON_X = 50;
const int MEDIUM_BUTTON_X = 150;
const int HARD_BUTTON_X = 250;
const int GENERATE_BUTTON_X = 350;
const int SOLVE_BUTTON_X = 450;
const int CLOSE_BUTTON_X = 550;
const int BUTTON_Y = WINDOW_HEIGHT - BUTTON_HEIGHT - 10;

namespace suduko
{
    int Num = standard;
}