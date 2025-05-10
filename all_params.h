#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\数独盘.txt";
// 下面是solve页面的一些ui设计参数
const int lowerheight = 50;

const int CELL_SIZE = 60;
const int WINDOW_WIDTH = CELL_SIZE * standard;
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;
// 求解按钮的大小和位置
const int SOLVE_BUTTON_WIDTH = 80;
const int SOLVE_BUTTON_HEIGHT = 30;
const int SOLVE_BUTTON_X = (WINDOW_WIDTH - SOLVE_BUTTON_WIDTH) / 2 - 100;
const int SOLVE_BUTTON_Y = WINDOW_HEIGHT - SOLVE_BUTTON_HEIGHT - 10;
// 关闭按钮的大小及位置
const int CLOSE_BUTTON_WIDTH = 80;
const int CLOSE_BUTTON_HEIGHT = 30;
const int CLOSE_BUTTON_X = (WINDOW_WIDTH - CLOSE_BUTTON_WIDTH) / 2 + 100;
const int CLOSE_BUTTON_Y = WINDOW_HEIGHT - CLOSE_BUTTON_HEIGHT - 10;

namespace suduko
{
	int Num = standard;
}

