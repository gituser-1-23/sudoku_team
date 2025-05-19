#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "all_params.h"
#include "Mouse_ui.h"
#include "page_of_solve.h"
using namespace std;

string filename = suduko_file;
int N = suduko::Num;
vector<vector<int>> board(N, vector<int>(N, 0));
vector<vector<bool>> isInitial(N, vector<bool>(N, false)); // ��¼��ʼ�ǿհ׸���
int difficulty = 0; // 0-��, 1-�е�, 2-����
int blankCount = 0;

// ����� (row, col) λ�÷������� num �Ƿ�Ϸ�
bool isSafe(vector<vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) return false;
    }
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) return false;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) return false;
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
                        if (solveSudoku(board)) return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// ��鵱ǰ�����Ƿ�Ϊ��Ч��
bool isValidSudoku(vector<vector<int>>& board) {
    for (int i = 0; i < N; i++) {
        vector<bool> rowCheck(N + 1, false), colCheck(N + 1, false), boxCheck(N + 1, false);
        for (int j = 0; j < N; j++) {
            // �����
            if (board[i][j] < 1 || board[i][j] > 9 || rowCheck[board[i][j]]) return false;
            rowCheck[board[i][j]] = true;
            // �����
            if (board[j][i] < 1 || board[j][i] > 9 || colCheck[board[j][i]]) return false;
            colCheck[board[j][i]] = true;
            // ���3x3����
            int row = (i / 3) * 3 + j / 3;
            int col = (i % 3) * 3 + j % 3;
            int num = board[row][col];
            if (num < 1 || num > 9 || boxCheck[num]) return false;
            boxCheck[num] = true;
        }
    }
    return true;
}

// ����������������
bool generateSudoku(vector<vector<int>>& board, int row = 0, int col = 0) {
    if (row == N) return true;
    if (col == N) return generateSudoku(board, row + 1, 0);
    if (board[row][col] != 0) return generateSudoku(board, row, col + 1);

    vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (size_t i = nums.size() - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        std::swap(nums[i], nums[j]);
    }

    for (int num : nums) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (generateSudoku(board, row, col + 1)) return true;
            board[row][col] = 0;
        }
    }
    return false;
}

// �Ƴ�ָ�����������������ɲ�ͬ�ѶȲ���¼��ʼ״̬
void removeNumbers(vector<vector<int>>& board, int count) {
    int removed = 0;
    isInitial.assign(N, vector<bool>(N, true)); // ��ʼȫΪ�ǿհ�
    while (removed < count) {
        int row = rand() % N;
        int col = rand() % N;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            isInitial[row][col] = false; // ���Ϊ�հ׸���
            removed++;
        }
    }
}

// ���ļ��ж�ȡ�������̣����� # ��ʾ��ȱ��
vector<vector<int>> readSudokuFromFile(const string& filename) {
    vector<vector<int>> board(N, vector<int>(N, 0));
    isInitial.assign(N, vector<bool>(N, false)); // Ĭ��ȫΪ�հ׸���
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        size_t row = 0;
        while (getline(file, line) && row < static_cast<size_t>(N)) {
            size_t col = 0;
            for (char c : line) {
                if (c == ' ') continue;
                if (c == '#') {
                    board[row][col] = 0;
                    isInitial[row][col] = false;
                }
                else if (isdigit(c)) {
                    board[row][col] = c - '0';
                    isInitial[row][col] = true;
                }
                else {
                    cerr << "Invalid character in file: " << c << endl;
                    isInitial.assign(N, vector<bool>(N, false));
                    return board;
                }
                col++;
            }
            if (col != static_cast<size_t>(N)) {
                cerr << "Invalid line length in file: " << line << endl;
                isInitial.assign(N, vector<bool>(N, false));
                return board;
            }
            row++;
        }
        if (row != static_cast<size_t>(N)) {
            cerr << "Invalid number of lines in file." << endl;
            isInitial.assign(N, vector<bool>(N, false));
            return board;
        }
        file.close();
    }
    return board;
}

// �����Ѷ�ѡ��ť����ⰴť
void drawButtons(int mouseX, int mouseY) {
    bool easyHovered = isButtonHovered(mouseX, mouseY, EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool mediumHovered = isButtonHovered(mouseX, mouseY, MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool hardHovered = isButtonHovered(mouseX, mouseY, HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool generateHovered = isButtonHovered(mouseX, mouseY, GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool solveHovered = isButtonHovered(mouseX, mouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    bool closeHovered = isButtonHovered(mouseX, mouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);

    drawButton(EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("��"), easyHovered);
    drawButton(MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("�е�"), mediumHovered);
    drawButton(HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("����"), hardHovered);
    drawButton(GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("����"), generateHovered);
    drawButton(SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("���"), solveHovered);
    drawButton(CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, _T("�ر�"), closeHovered);
}

// ��������Ƿ�����
bool isBoardFull(vector<vector<int>>& board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) return false;
        }
    }
    return true;
}

// �Զ��������������ʱʹ�ÿ���̨���룩
int customInputBox(const char* prompt, char* buffer, int bufferSize) {
    string input;
    cout << prompt << endl;
    getline(cin, input);
    if (input.length() > 0 && input.length() <= static_cast<size_t>(bufferSize - 1)) {
        strcpy_s(buffer, bufferSize, input.c_str());
        return 1; // ģ��ɹ�
    }
    return 0; // ģ��ȡ��
}

int show() {
    srand((unsigned)time(0));
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(BLACK); // ���ñ���Ϊ��ɫ
    cleardevice();    // �����Ļ
    board = readSudokuFromFile(filename);

    // ����ƫ������ʹ���̾���
    int gridWidth = N * CELL_SIZE;
    int gridHeight = N * CELL_SIZE;
    int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - gridHeight - lowerheight) / 2;

    // ��������ԭ��ƫ�ƣ�ʹ���̾���
    setorigin(offsetX, offsetY);

    int selectedRow = -1, selectedCol = -1; // ��¼��ǰѡ�еĸ���
    drawBoard(board, isInitial, selectedRow, selectedCol);

    MOUSEMSG msg;
    bool flag = true;
    int lastMouseX = 0, lastMouseY = 0; // ��¼��һ�����λ��

    while (flag) {
        // ���������Ϣ
        if (MouseHit()) {
            msg = GetMouseMsg();
            lastMouseX = msg.x - offsetX;
            lastMouseY = msg.y - offsetY;
            switch (msg.uMsg) {
            case WM_MOUSEMOVE:
                drawButtons(lastMouseX, lastMouseY);
                break;
            case WM_LBUTTONDOWN:
                // �Ѷ�ѡ��
                if (isButtonClicked(lastMouseX, lastMouseY, EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 0; blankCount = 35;
                    selectedRow = selectedCol = -1; // ȡ��ѡ��
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 1; blankCount = 45;
                    selectedRow = selectedCol = -1;
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    difficulty = 2; blankCount = 55;
                    selectedRow = selectedCol = -1;
                }
                // ����������
                else if (isButtonClicked(lastMouseX, lastMouseY, GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    board.assign(N, vector<int>(N, 0));
                    generateSudoku(board);
                    removeNumbers(board, blankCount);
                    cleardevice();
                    setorigin(offsetX, offsetY);
                    selectedRow = selectedCol = -1;
                    drawBoard(board, isInitial, selectedRow, selectedCol);
                }
                // �������
                else if (isButtonClicked(lastMouseX, lastMouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    if (solveSudoku(board)) {
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        selectedRow = selectedCol = -1;
                        drawBoard(board, isInitial, selectedRow, selectedCol);
                    }
                    else {
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        settextcolor(RGB(255, 0, 0));
                        settextstyle(30, 0, _T("����"));
                        outtextxy(gridWidth / 2 - 100, gridHeight / 2 - 15, _T("�޽��������"));
                    }
                }
                // �رճ���
                else if (isButtonClicked(lastMouseX, lastMouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    closegraph();
                    flag = false;
                }
                // ������̸���
                else {
                    int row = (msg.y - offsetY) / CELL_SIZE;
                    int col = (msg.x - offsetX) / CELL_SIZE;
                    if (row >= 0 && row < N && col >= 0 && col < N) {
                        // ���Կ��޸ĸ��Ӳ���
                        if (!isInitial[row][col]) {
                            // ������������ѡ�еĸ��ӣ����������
                            if (selectedRow == row && selectedCol == col) {
                                board[row][col] = 0;
                                selectedRow = selectedCol = -1; // ȡ��ѡ��
                            }
                            else {
                                // ѡ���¸���
                                selectedRow = row;
                                selectedCol = col;

                                // ���������
                                char input[10];
                                int result = customInputBox("���������� (1-9):", input, 10);
                                if (result == 1 && strlen(input) == 1 && isdigit(input[0])) {
                                    int number = input[0] - '0';
                                    if (number >= 1 && number <= 9) {
                                        board[selectedRow][selectedCol] = number;
                                    }
                                }
                            }
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, selectedRow, selectedCol);
                            drawButtons(lastMouseX, lastMouseY);
                            if (isBoardFull(board)) {
                                settextstyle(30, 0, _T("����"));
                                if (isValidSudoku(board)) {
                                    settextcolor(RGB(0, 255, 0));
                                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("��ʤ"));
                                }
                                else {
                                    settextcolor(RGB(255, 0, 0));
                                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("ʧ��"));
                                }
                            }
                        }
                    }
                    else {
                        // ��������⣬ȡ��ѡ��
                        selectedRow = selectedCol = -1;
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, selectedRow, selectedCol);
                        drawButtons(lastMouseX, lastMouseY);
                    }
                }
                break;
            case WM_CLOSE:
                closegraph();
                flag = false;
                break;
            }
        }
    }
    return 0;
}