#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\������.txt";
// ������solveҳ���һЩui��Ʋ���
const int lowerheight = 50;

const int CELL_SIZE = 60;
const int WINDOW_WIDTH = CELL_SIZE * standard;
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;
// ��ⰴť�Ĵ�С��λ��
const int SOLVE_BUTTON_WIDTH = 80;
const int SOLVE_BUTTON_HEIGHT = 30;
const int SOLVE_BUTTON_X = (WINDOW_WIDTH - SOLVE_BUTTON_WIDTH) / 2 - 100;
const int SOLVE_BUTTON_Y = WINDOW_HEIGHT - SOLVE_BUTTON_HEIGHT - 10;
// �رհ�ť�Ĵ�С��λ��
const int CLOSE_BUTTON_WIDTH = 80;
const int CLOSE_BUTTON_HEIGHT = 30;
const int CLOSE_BUTTON_X = (WINDOW_WIDTH - CLOSE_BUTTON_WIDTH) / 2 + 100;
const int CLOSE_BUTTON_Y = WINDOW_HEIGHT - CLOSE_BUTTON_HEIGHT - 10;

namespace suduko
{
	int Num = standard;
}

