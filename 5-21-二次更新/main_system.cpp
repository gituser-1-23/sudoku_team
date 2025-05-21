#include<iostream>
#include"solution_of_Suduko.h"
#include"players_num.h"
#include"create_players.h"
#include"scores_of_all_players.h"
#include"player_system.h"
#include"sort_the_players.h"
#include "keyboard_input_handler.h"  
#include "page_of_solve.h"

using namespace std;

int selectedRow = -1;
int selectedCol = -1;
extern int selectedRow, selectedCol;
extern std::vector<std::vector<int>> board;
extern std::vector<std::vector<bool>> isInitial;

int main()
{
    try
    {
        // ��ʼ��ͼ�δ���
        srand((unsigned)time(0));
        initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
        setbkcolor(BLACK);
        cleardevice();

        // ���������ļ�
        board = readSudokuFromFile(suduko_file);  // ���ļ���ȡ����
        int N = suduko::Num;
        isInitial.assign(N, vector<bool>(N, false));  // ��ʼ������״̬

        // UI����ƫ��
        int gridWidth = N * CELL_SIZE;
        int gridHeight = N * CELL_SIZE;
        int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
        int offsetY = (WINDOW_HEIGHT - gridHeight - lowerheight) / 2;
        setorigin(offsetX, offsetY);

        int selectedRow = -1, selectedCol = -1;
        drawBoard(board, isInitial, selectedRow, selectedCol);

        MOUSEMSG msg;
        bool running = true;
        int lastMouseX = 0, lastMouseY = 0;
        int difficulty = 0;
        int blankCount = 35; // Ĭ�ϼ�

        while (running)
        {
            if (MouseHit())
            {
                msg = GetMouseMsg();
                lastMouseX = msg.x - offsetX;
                lastMouseY = msg.y - offsetY;

                switch (msg.uMsg) {
                case WM_MOUSEMOVE:
                    drawButtons(lastMouseX, lastMouseY);  // ����ƶ��¼�
                    break;

                case WM_LBUTTONDOWN:  // �����������¼�
                    if (isButtonClicked(lastMouseX, lastMouseY, EASY_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        difficulty = 0;
                        blankCount = 35;
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, MEDIUM_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        difficulty = 1;
                        blankCount = 45;
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, HARD_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        difficulty = 2;
                        blankCount = 55;
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, GENERATE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        board.assign(N, vector<int>(N, 0));
                        generateSudoku(board);
                        removeNumbers(board, blankCount);
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, -1, -1);
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        if (solveSudoku(board)) {
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, -1, -1);
                        }
                        else {
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            settextcolor(RGB(255, 0, 0));
                            settextstyle(30, 0, _T("����"));
                            outtextxy(gridWidth / 2 - 100, gridHeight / 2 - 15, _T("�޽��������"));
                        }
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        running = false;
                        closegraph();
                    }
                    else {
                        int row = (msg.y - offsetY) / CELL_SIZE;
                        int col = (msg.x - offsetX) / CELL_SIZE;
                        if (row >= 0 && row < N && col >= 0 && col < N) {
                            if (!isInitial[row][col]) {
                                if (selectedRow == row && selectedCol == col) {
                                    board[row][col] = 0;
                                    selectedRow = selectedCol = -1;
                                }
                                else {
                                    selectedRow = row;
                                    selectedCol = col;
                                }
                                cleardevice();
                                setorigin(offsetX, offsetY);
                                drawBoard(board, isInitial, selectedRow, selectedCol);
                                drawButtons(lastMouseX, lastMouseY);

                                if (isBoardFull(board)) {
                                    settextstyle(30, 0, _T("����"));
                                    if (isValidSudoku(board)) {
                                        settextcolor(RGB(0, 255, 0));
                                        outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("��ʤ��"));
                                    }
                                    else {
                                        settextcolor(RGB(255, 0, 0));
                                        outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("ʧ�ܣ�"));
                                    }
                                }
                            }
                        }
                        else {
                            selectedRow = selectedCol = -1;
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, selectedRow, selectedCol);
                            drawButtons(lastMouseX, lastMouseY);
                        }
                    }
                    break;

                case WM_KEYDOWN:
                    handleKeyboardInput(board, isInitial, selectedRow, selectedCol, offsetX, offsetY);  // ����ƫ����
                    break;

                case WM_CLOSE:
                    running = false;
                    closegraph();
                    break;
                }
            }

            // ����ֱ��������ƣ������Ʒǳ�ʼ���ӣ�
            if (selectedRow != -1 && selectedCol != -1 && !isInitial[selectedRow][selectedCol]) {
                for (char ch = '1'; ch <= '9'; ++ch) {
                    if (GetAsyncKeyState(ch) & 0x8000) {
                        if (!isInitial[selectedRow][selectedCol]) {
                            board[selectedRow][selectedCol] = ch - '0';
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

                            Sleep(150);
                            break;
                        }
                    }
                }

                if ((GetAsyncKeyState(VK_DELETE) & 0x8000) || (GetAsyncKeyState(VK_BACK) & 0x8000)) {
                    if (!isInitial[selectedRow][selectedCol]) {
                        board[selectedRow][selectedCol] = 0;
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, selectedRow, selectedCol);
                        drawButtons(lastMouseX, lastMouseY);
                        Sleep(150);
                    }
                }
            }
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << "����: " << e.what() << std::endl;
        return 1;
    }
}
