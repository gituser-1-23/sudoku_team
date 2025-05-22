// main_system.cpp���޸ĺ�
#include<iostream>
#include "main_menu.h" // ���˵�ͷ�ļ�
#include "solution_of_Suduko.h"
#include "players_num.h"
#include "create_players.h"
#include "scores_of_all_players.h"
#include "player_system.h"
#include "sort_the_players.h"
#include "keyboard_input_handler.h"  
#include "page_of_solve.h"

using namespace std;

// ȫ�ֱ�������
int selectedRow = -1;
int selectedCol = -1;
vector<vector<int>> board;
vector<vector<bool>> isInitial;

// ��ʼ��Ϸ�߼�
void startGame() {
    // ԭ��Ϸ�߼���ʼ��
    srand((unsigned)time(0));
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(BLACK);
    cleardevice();

    board = readSudokuFromFile(suduko_file);
    int N = suduko::Num;
    isInitial.assign(N, vector<bool>(N, false));

    int gridWidth = N * CELL_SIZE;
    int gridHeight = N * CELL_SIZE;
    int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - gridHeight - lowerheight) / 2;
    setorigin(offsetX, offsetY);

    // ԭ��Ϸѭ��
    bool running = true;
    int lastMouseX = 0, lastMouseY = 0;
    int difficulty = 0;
    int blankCount = 35;

    drawBoard(board, isInitial, selectedRow, selectedCol);
    MOUSEMSG msg;

    while (running) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            lastMouseX = msg.x - offsetX;
            lastMouseY = msg.y - offsetY;

            switch (msg.uMsg) {
            case WM_MOUSEMOVE:
                drawButtons(lastMouseX, lastMouseY);  // ����ƶ��¼�
                break;

            case WM_LBUTTONDOWN:  // ����������¼�
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
                        outtextxy(gridWidth / 2 - 100, gridHeight / 2 - 15, _T("�޽⣬����������"));
                    }
                }
                else if (isButtonClicked(lastMouseX, lastMouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    running = false;
                    closegraph();
                }
                else {
                    int row = (msg.y - offsetY) / CELL_SIZE;
                    int col = (msg.x - offsetX) / CELL_SIZE;
                    if (row >= 0 && row < N && col >= 0 && col < N)
                    {
                        if (!isInitial[row][col])
                        {
                            if (selectedRow == row && selectedCol == col)
                            {
                                board[row][col] = 0;
                                selectedRow = selectedCol = -1;
                            }
                            else
                            {
                                selectedRow = row;
                                selectedCol = col;
                            }
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, selectedRow, selectedCol);
                            drawButtons(lastMouseX, lastMouseY);

                            if (isBoardFull(board))
                            {
                                settextstyle(30, 0, _T("����"));
                                if (isValidSudoku(board))
                                {
                                    settextcolor(RGB(0, 255, 0));
                                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("��ս�ɹ���"));
                                }
                                else
                                {
                                    settextcolor(RGB(255, 0, 0));
                                    outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("��սʧ�ܣ�"));
                                }
                            }
                        }
                    }
                    else
                    {
                        selectedRow = selectedCol = -1;
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, selectedRow, selectedCol);
                        drawButtons(lastMouseX, lastMouseY);
                    }
                }
                break;

            case WM_CLOSE:
                running = false;
                closegraph();
                break;
            }
        }
        handleKeyboardInput(board, isInitial, selectedRow, selectedCol, offsetX, offsetY, gridWidth, gridHeight, lastMouseX, lastMouseY);
    }
    closegraph();
}

// ��ʾ��¼��ע�����
void showLoginAndRegister(PlayerService& playerService) {
    initgraph(300, 300);
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(18, 0, _T("����"));

    string name, code;
    while (true) {
        cleardevice();
        outtextxy(50, 50, _T("�û���: "));
        outtextxy(50, 100, _T("����: "));
        outtextxy(50, 150, _T("1. ��¼"));
        outtextxy(50, 200, _T("2. ע��"));
        outtextxy(50, 250, _T("3. �������˵�"));

        // ��ȡ�û�����
        name.clear();
        code.clear();
        outtextxy(150, 50, _T(""));
        outtextxy(150, 100, _T(""));
        int x = 150;
        int y = 50;
        char ch;
        while (true) {
            if (_kbhit()) {
                ch = _getch();
                if (ch == '\r') break;
                else if (ch == '\b') {
                    if (!name.empty()) {
                        name.pop_back();
                        x -= 10;
                        outtextxy(x, y, _T(" "));
                    }
                }
                else {
                    name += ch;
                    outtextxy(x, y, ch);
                    x += 10;
                }
            }
        }
        x = 150;
        y = 100;
        while (true) {
            if (_kbhit()) {
                ch = _getch();
                if (ch == '\r') break;
                else if (ch == '\b') {
                    if (!code.empty()) {
                        code.pop_back();
                        x -= 10;
                        outtextxy(x, y, _T(" "));
                    }
                }
                else {
                    code += ch;
                    outtextxy(x, y, _T("*"));
                    x += 10;
                }
            }
        }

        // ��ȡ�û�ѡ��
        while (!_kbhit());
        ch = _getch();
        if (ch == '1') {
            if (playerService.login(name, code)) {
                cout << "��¼�ɹ���" << endl;
                closegraph();
                return;
            }
            else {
                cout << "��¼ʧ�ܣ������û��������룡" << endl;
            }
        }
        else if (ch == '2') {
            playerService.registerPlayer(name, code);
            cout << "ע��ɹ������¼��" << endl;
        }
        else if (ch == '3') {
            closegraph();
            return;
        }
    }
}

int main() {
    try {
        // ��ʼ�����˵�����
        initgraph(500, 500);
        setbkcolor(BLACK);
        cleardevice();

        MainMenuOption option;
        PlayerService playerService;

        while (true) {
            drawMainMenu();
            option = handleMenuInput();

            switch (option) {
            case OPTION_CHALLENGE:
                closegraph(); // �ر����˵�����
                startGame();  // ��ʼ��Ϸ
                initgraph(500, 500); // ���´����˵�����
                break;
            case OPTION_SCOREBOARD:
                showScoreboard();
                cleardevice();
                break;
            case OPTION_EXIT:
                closegraph();
                return 0;
            case OPTION_ACCOUNT:
                showLoginAndRegister(playerService);
                cleardevice();
                break;
            }
        }
    }
    catch (const exception& e) {
        cerr << "����: " << e.what() << endl;
        closegraph();
        return 1;
    }
}