#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include"all_params.h"
#include"Mouse_ui.h"
#include"page_of_solve.h"
using namespace std;
string filename = suduko_file;

int N = suduko::Num;
// ����� (row, col) λ�÷������� num �Ƿ�Ϸ�
bool isSafe(vector<vector<int>>& board, int row, int col, int num)
{
    // �����
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) {
            return false;
        }
    }

    // �����
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }

    // ��� 3x3 С����
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

// �����㷨�������
bool solveSudoku(vector<vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= N; ++num) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// ���ļ��ж�ȡ�������̣����� # ��ʾ��ȱ��
vector<vector<int>> readSudokuFromFile(const string& filename)
{
    vector<vector<int>> board(N, vector<int>(N, 0));
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        int row = 0;
        while (getline(file, line) && row < N) {
            int col = 0;
            for (char c : line) {
                if (c == ' ') continue; // �����ո�
                if (c == '#') {
                    board[row][col] = 0;
                }
                else if (isdigit(c)) {
                    board[row][col] = c - '0';
                }
                else {
                    cerr << "Invalid character in file: " << c << endl;
                    return {};
                }
                col++;
            }
            if (col != N) {
                cerr << "Invalid line length in file: " << line << endl;
                return {};
            }
            row++;
        }
        if (row != N) {
            cerr << "Invalid number of lines in file." << endl;
            return {};
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
        return {};
    }
    return board;
}

int show()
{
    vector<vector<int>> board = readSudokuFromFile(filename);
    if (board.empty())
    {
        return 1;
    }

    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    drawBoard(board);

    MOUSEMSG msg;
    bool flag = true;
    while (flag) {
        msg = GetMouseMsg();

        bool solveButtonHovered = isButtonHovered(msg.x, msg.y, SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT);
        bool closeButtonHovered = isButtonHovered(msg.x, msg.y, CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT);

        drawButton(SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT, _T("�������"), solveButtonHovered);
        drawButton(CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT, _T("�رճ���"), closeButtonHovered);

        switch (msg.uMsg) {
        case WM_MOUSEMOVE:
            break;
        case WM_LBUTTONDOWN:
            if (isButtonClicked(msg.x, msg.y, SOLVE_BUTTON_X, SOLVE_BUTTON_Y, SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_HEIGHT)) {
                if (solveSudoku(board)) {
                    cleardevice();
                    drawBoard(board);
                }
                else {
                    cleardevice();
                    settextcolor(RED);
                    settextstyle(30, 0, _T("����"));
                    outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 15, _T("�޽��������"));
                }
            }
            else if (isButtonClicked(msg.x, msg.y, CLOSE_BUTTON_X, CLOSE_BUTTON_Y, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT)) {
                closegraph();
                flag = false;
            }
            break;
        case WM_CLOSE:
            closegraph();
            flag = false;
            break;
        }
    }
    return 0;
}