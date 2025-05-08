#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>

using namespace std;

const int lowerheight = 50;
const int N = 9;
const int CELL_SIZE = 60;
const int WINDOW_WIDTH = CELL_SIZE * N;
const int WINDOW_HEIGHT = CELL_SIZE * N + lowerheight;


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

// ����� (row, col) λ�÷������� num �Ƿ�Ϸ�
bool isSafe(vector<vector<int>>& board, int row, int col, int num) {
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

// �����������̺�����
void drawBoard(const vector<vector<int>>& board) {
    // ����������
    for (int i = 0; i <= N; ++i) {
        int lineWidth = (i % 3 == 0) ? 3 : 1;
        setlinestyle(PS_SOLID, lineWidth);
        line(0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
        line(i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT - lowerheight);
    }

    // ��������
    settextcolor(WHITE);
    settextstyle(30, 0, _T("����"));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] != 0) {
                TCHAR num[2];
                _stprintf_s(num, _T("%d"), board[i][j]);
                int x = j * CELL_SIZE + CELL_SIZE / 2 - 10;
                int y = i * CELL_SIZE + CELL_SIZE / 2 - 15;
                outtextxy(x, y, num);
            }
        }
    }
}
// ���ư�ť
void drawButton(int x, int y, int width, int height, const TCHAR* text, bool isHovered) {
    COLORREF fillColor = isHovered ? RGB(122, 122, 122) : RGB(92, 92, 92);
    setfillcolor(fillColor);
    fillrectangle(x, y, x + width, y + height);
    settextcolor(WHITE);
    settextstyle(18, 0, _T("����"));
    outtextxy(x + 5, y + 5, text);
}

// �ж�������Ƿ��ڰ�ť��
bool isButtonClicked(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

// �ж�����Ƿ���ͣ�ڰ�ť��
bool isButtonHovered(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return isButtonClicked(x, y, buttonX, buttonY, buttonWidth, buttonHeight);
}



int main()
{
    string filename = ".\\������.txt";
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