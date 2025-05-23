#include "solution_of_Suduko.h"
#include "players_num.h"
#include "create_players.h"
#include "scores_of_all_players.h"
#include "player_system.h"
#include "sort_the_players.h"
#include "keyboard_input_handler.h"
#include "page_of_solve.h"
#include "main_menu.h"


#include <chrono>
#include <iostream>
using namespace std::chrono;
using namespace std;

// 这些全局变量只能定义一次，避免头文件重复定义
int selectedRow = -1;
int selectedCol = -1;
int hintRow = -1;
int hintCol = -1;

TCHAR resultMessage[64] = _T("");         // 结果显示信息
COLORREF resultColor = RGB(255, 255, 255); // 结果显示颜色

vector<vector<int>> board;
vector<vector<bool>> isInitial;

std::string filename = suduko_file;  // 文件名在 all_params.h 中定义
int N = suduko::Num;
int difficulty = 0;
int blankCount = 35;

steady_clock::time_point startTime;
bool timing = false;  // 是否正在计时
duration<int> elapsedAtStop(0);  // 记录停止时的时间

void startSudokuGame()
{
    try
    {
        srand((unsigned)time(0));
        initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
        setbkcolor(BLACK);
        cleardevice();

        board = readSudokuFromFile(suduko_file);  // 从文件读取数独
        int N = suduko::Num;
        isInitial.assign(N, vector<bool>(N, false));

        int gridWidth = N * CELL_SIZE;
        int gridHeight = N * CELL_SIZE;
        int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
        int offsetY = (WINDOW_HEIGHT - gridHeight - lowerheight) / 2;
        setorigin(offsetX, offsetY);

        drawBoard(board, isInitial, selectedRow, selectedCol);

        MOUSEMSG msg;
        bool running = true;
        int lastMouseX = 0, lastMouseY = 0;
        int difficulty = 0;
        int blankCount = 35;

        while (running)
        {
            if (MouseHit())
            {
                msg = GetMouseMsg();
                lastMouseX = msg.x - offsetX;
                lastMouseY = msg.y - offsetY;

                switch (msg.uMsg) {
                case WM_MOUSEMOVE:
                    drawButtons(lastMouseX, lastMouseY);  // 鼠标移动事件
                    break;
                case WM_LBUTTONDOWN:  // 鼠标左键点击事件
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
                        startTime = steady_clock::now();
                        timing = true;
                        selectedRow = selectedCol = -1;   // 清除高亮选择
                        hintRow = hintCol = -1;           // 清除高亮提示区域
                        _tcscpy_s(resultMessage, _T(""));
                        resultColor = RGB(255, 255, 255);
                        cleardevice();
                        setorigin(offsetX, offsetY);
                        drawBoard(board, isInitial, -1, -1, timing, -1, -1);
                        drawButtons(lastMouseX, lastMouseY);
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, SOLVE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        if (solveSudoku(board)) {
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, -1, -1, timing, -1, -1);
                            drawButtons(lastMouseX, lastMouseY);
                        }
                        else {
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            settextcolor(RGB(255, 0, 0));
                            settextstyle(30, 0, _T("宋体"));
                            outtextxy(gridWidth / 2 - 100, gridHeight / 2 - 15, _T("无解，请重新生成"));
                        }
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, END_BUTTON_X, END_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        if (timing) {
                            auto now = steady_clock::now();
                            elapsedAtStop = duration_cast<seconds>(now - startTime);
                            timing = false;
                        }
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, CLOSE_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        running = false;
                        closegraph();
                    }
                    else if (isButtonClicked(lastMouseX, lastMouseY, HINT_BUTTON_X, HINT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                        vector<vector<int>> solvedBoard = board;
                        if (solveSudoku(solvedBoard)) {
                            vector<pair<int, int>> emptyCells;
                            for (int i = 0; i < N; ++i) {
                                for (int j = 0; j < N; ++j) {
                                    if (!isInitial[i][j] && board[i][j] == 0) {
                                        emptyCells.emplace_back(i, j);
                                    }
                                }
                            }
                            if (!emptyCells.empty()) {
                                int idx = rand() % emptyCells.size();
                                int r = emptyCells[idx].first;
                                int c = emptyCells[idx].second;
                                board[r][c] = solvedBoard[r][c];
                                hintRow = r;
                                hintCol = c;
                                cleardevice();
                                setorigin(offsetX, offsetY);
                                drawBoard(board, isInitial, selectedRow, selectedCol, timing, hintRow, hintCol);
                                drawButtons(lastMouseX, lastMouseY);
                            }
                        }
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
                                drawBoard(board, isInitial, selectedRow, selectedCol, timing, hintRow, hintCol);
                                drawButtons(lastMouseX, lastMouseY);
                                if (isBoardFull(board)) {
                                    settextstyle(30, 0, _T("宋体"));
                                    if (isValidSudoku(board)) {
                                        settextcolor(RGB(0, 255, 0));
                                        outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("填写正确！"));
                                    }
                                    else {
                                        settextcolor(RGB(255, 0, 0));
                                        outtextxy(gridWidth / 2 - 50, gridHeight / 2 - 15, _T("填写错误！"));
                                    }
                                }
                            }
                        }
                        else {
                            selectedRow = selectedCol = -1;
                            cleardevice();
                            setorigin(offsetX, offsetY);
                            drawBoard(board, isInitial, selectedRow, selectedCol, timing, hintRow, hintCol);
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

            static auto lastUpdate = steady_clock::now();
            auto now = steady_clock::now();
            if (duration_cast<milliseconds>(now - lastUpdate).count() >= 100) {
                cleardevice();
                setorigin(offsetX, offsetY);
                drawBoard(board, isInitial, selectedRow, selectedCol, timing, hintRow, hintCol);
                drawButtons(lastMouseX, lastMouseY);
            }
            lastUpdate = now;
        }
    }
    catch (const exception& e)
    {
        cerr << "An error occurred: " << e.what() << endl;
    }
}

int main()
{
    // 显示主菜单
    drawMainMenu();
    MainMenuOption option = handleMenuInput();

    switch (option)
    {
    case OPTION_CHALLENGE:
        startSudokuGame();
        break;
    case OPTION_SCOREBOARD:
        showScoreboard();
        break;
    case OPTION_EXIT:
        return 0;
    case OPTION_ACCOUNT:
        // 处理账户相关逻辑
        break;
    default:
        break;
    }

    return 0;
}