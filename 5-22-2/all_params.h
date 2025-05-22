#pragma once
#include <iostream>
#include <string>
using namespace std;

// === 基本参数 ===
const int standard = 9;
const string suduko_file = ".\\数独盘.txt";
const string folder_path = ".\\players\\";

// === UI界面设计参数 ===
const int lowerheight = 50;
const int CELL_SIZE = 60;

// === 计算窗口尺寸 ===
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// === 按钮尺寸和位置参数 ===
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// 按钮纵坐标（底部对齐）
const int BUTTON_Y = WINDOW_HEIGHT - BUTTON_HEIGHT - 10;
const int END_BUTTON_Y = BUTTON_Y - BUTTON_HEIGHT - 5;
const int HINT_BUTTON_Y = END_BUTTON_Y - BUTTON_HEIGHT - 5;

// 按钮横坐标（左对齐）
const int EASY_BUTTON_X = 50;
const int MEDIUM_BUTTON_X = 150;
const int HARD_BUTTON_X = 250;
const int GENERATE_BUTTON_X = 350;
const int SOLVE_BUTTON_X = 450;
const int END_BUTTON_X = 550;
const int CLOSE_BUTTON_X = 650;
const int HINT_BUTTON_X = END_BUTTON_X;

// === 命名空间 ===
namespace suduko {
    static int Num = standard;
}
