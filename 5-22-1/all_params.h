#pragma once
#include<iostream>
#include<string>
using namespace std;

const int standard = 9;
string suduko_file = ".\\������.txt";
string folder_path = ".\\players\\"; // ����ļ���·��

// ������solveҳ���һЩUI��Ʋ���
const int lowerheight = 50;
const int CELL_SIZE = 60;

// ���㴰�ڳߴ�
const int WINDOW_WIDTH = 1000;  // �����������ɰ�ť
const int WINDOW_HEIGHT = CELL_SIZE * standard + lowerheight;

// ��ť��С
const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 30;

// ��ť Y ���꣨�������ϣ�
const int BUTTON_Y = WINDOW_HEIGHT - BUTTON_HEIGHT - 10;          // ����°�ť
const int END_BUTTON_Y = BUTTON_Y - BUTTON_HEIGHT - 5;            // ����������ť�����Ϸ�
const int HINT_BUTTON_Y = END_BUTTON_Y - BUTTON_HEIGHT - 5;       // ����ʾ����ť�ڡ��������Ϸ�

// ��ť X ���꣨ȫ������룩
const int EASY_BUTTON_X = 50;
const int MEDIUM_BUTTON_X = 150;
const int HARD_BUTTON_X = 250;
const int GENERATE_BUTTON_X = 350;
const int SOLVE_BUTTON_X = 450;
const int END_BUTTON_X = 550;
const int CLOSE_BUTTON_X = 650;
const int HINT_BUTTON_X = END_BUTTON_X;  // ����ʾ����ť�롰����������

namespace suduko
{
    int Num = standard;
}
