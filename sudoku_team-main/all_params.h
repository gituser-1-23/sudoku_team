#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\������.txt";
// ������solveҳ���һЩui��Ʋ���
const int lowerheight = 50;

const int CELL_SIZE = 60;
const int WINDOW_WIDTH = 700; // ���������������а�ť������
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// ��ť��С��λ�ã����µ�������Ӧ�´��ڿ�ȣ�
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// ��ťλ�ã�6����ť���򵥡��еȡ����ѡ����ɡ���⡢�رգ�
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