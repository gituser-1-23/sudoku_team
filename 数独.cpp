#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const int N = 9;

// 检查在 (row, col) 位置放置数字 num 是否合法
bool isSafe(std::vector<std::vector<int>>& board, int row, int col, int num) {
    // 检查行
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num) {
            return false;
        }
    }

    // 检查列
    for (int i = 0; i < N; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }

    // 检查 3x3 小宫格
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

// 回溯算法求解数独
bool solveSudoku(std::vector<std::vector<int>>& board) {
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

// 从文件中读取数独棋盘，处理 # 表示的缺项
std::vector<std::vector<int>> readSudokuFromFile(const std::string& filename) {
    std::vector<std::vector<int>> board(N, std::vector<int>(N, 0));
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < N) {
            int col = 0;
            for (char c : line) {
                if (c == ' ') continue; // 跳过空格
                if (c == '#') {
                    board[row][col] = 0;
                }
                else if (isdigit(c)) {
                    board[row][col] = c - '0';
                }
                else {
                    std::cerr << "Invalid character in file: " << c << std::endl;
                    return {};
                }
                col++;
            }
            if (col != N) {
                std::cerr << "Invalid line length in file: " << line << std::endl;
                return {};
            }
            row++;
        }
        if (row != N) {
            std::cerr << "Invalid number of lines in file." << std::endl;
            return {};
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return {};
    }
    return board;
}

// 打印数独棋盘
void printBoard(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string filename = "d:\\Users\\10447\\Desktop\\数独.txt";
    std::vector<std::vector<int>> board = readSudokuFromFile(filename);
    if (board.empty()) {
        return 1;
    }

    std::cout << "Original Sudoku:" << std::endl;
    printBoard(board);

    if (solveSudoku(board)) {
        std::cout << "\nSolved Sudoku:" << std::endl;
        printBoard(board);
    }
    else {
        std::cout << "\nNo solution exists." << std::endl;
    }

    return 0;
}