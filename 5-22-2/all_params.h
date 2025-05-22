#pragma once
#include <iostream>
#include <string>
using namespace std;

// === �������� ===
const int standard = 9;
const string suduko_file = ".\\������.txt";
const string folder_path = ".\\players\\";

// === UI������Ʋ��� ===
const int lowerheight = 50;
const int CELL_SIZE = 60;

// === ���㴰�ڳߴ� ===
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// === ��ť�ߴ��λ�ò��� ===
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// ��ť�����꣨�ײ����룩
const int BUTTON_Y = WINDOW_HEIGHT - BUTTON_HEIGHT - 10;
const int END_BUTTON_Y = BUTTON_Y - BUTTON_HEIGHT - 5;
const int HINT_BUTTON_Y = END_BUTTON_Y - BUTTON_HEIGHT - 5;

// ��ť�����꣨����룩
const int EASY_BUTTON_X = 50;
const int MEDIUM_BUTTON_X = 150;
const int HARD_BUTTON_X = 250;
const int GENERATE_BUTTON_X = 350;
const int SOLVE_BUTTON_X = 450;
const int END_BUTTON_X = 550;
const int CLOSE_BUTTON_X = 650;
const int HINT_BUTTON_X = END_BUTTON_X;

// === �����ռ� ===
namespace suduko {
    static int Num = standard;
}
