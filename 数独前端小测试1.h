// File: login_ui.h
#pragma once
#include <graphics.h>
#include "all_params.h"
#include "Mouse_ui.h"

namespace LoginUI {
    const int LOGIN_BTN_WIDTH = 100;
    const int LOGIN_BTN_HEIGHT = 40;
    const int LOGIN_BTN_X = (WINDOW_WIDTH - LOGIN_BTN_WIDTH) / 2;
    const int LOGIN_BTN_Y = 200;

    // 登录界面绘制与逻辑
    bool showLogin() {
        initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
        bool loginSuccess = false;

        // 绘制输入框和按钮
        settextstyle(20, 0, _T("宋体"));
        outtextxy(LOGIN_BTN_X - 120, 150, _T("用户名:"));
        outtextxy(LOGIN_BTN_X - 120, 250, _T("密码:"));

        // 交互逻辑
        MOUSEMSG msg;
        while (true) {
            msg = GetMouseMsg();
            bool loginHover = isButtonHovered(msg.x, msg.y, LOGIN_BTN_X, LOGIN_BTN_Y, LOGIN_BTN_WIDTH, LOGIN_BTN_HEIGHT);

            // 绘制动态按钮
            drawButton(LOGIN_BTN_X, LOGIN_BTN_Y, LOGIN_BTN_WIDTH, LOGIN_BTN_HEIGHT, _T("登录"), loginHover);

            if (msg.uMsg == WM_LBUTTONDOWN && loginHover) {
                loginSuccess = true; // 实际应验证输入
                break;
            }
        }
        closegraph();
        return loginSuccess;
    }
}
// File: difficulty_select.h
#pragma once
#include <graphics.h>
#include "all_params.h"
#include "Mouse_ui.h"

namespace Difficulty {
    enum Level { EASY, MEDIUM, HARD };

    // 难度选择界面
    Level selectDifficulty() {
        initgraph(400, 300);
        Level selected = EASY;

        // 按钮参数
        const int BTN_WIDTH = 120;
        const int BTN_HEIGHT = 50;
        const int BTN_Y_START = 100;

        // 绘制按钮
        MOUSEMSG msg;
        while (true) {
            cleardevice();
            settextstyle(25, 0, _T("宋体"));
            outtextxy(150, 50, _T("选择难度"));

            // 三个难度按钮
            bool easyHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START, BTN_WIDTH, BTN_HEIGHT, _T("简单"), easyHover);

            bool medHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START + 70, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START + 70, BTN_WIDTH, BTN_HEIGHT, _T("中等"), medHover);

            bool hardHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START + 140, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START + 140, BTN_WIDTH, BTN_HEIGHT, _T("困难"), hardHover);

            // 处理点击
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (easyHover) return EASY;
                if (medHover) return MEDIUM;
                if (hardHover) return HARD;
            }
            Sleep(10); // 减少CPU占用
        }
    }
}
// File: game_ui.h
#pragma once
#include "page_of_solve.h"
#include "solution_of_Suduko.h"
#include "ranking_system.h"

namespace GameUI {
    // 整合游戏主界面
    void startGame(Difficulty::Level level) {
        // 根据难度加载不同数独文件
        string file = (level == Difficulty::EASY) ? "easy.txt" :
            (level == Difficulty::MEDIUM) ? "medium.txt" : "hard.txt";
        suduko_file = file; // 更新全局文件名

        // 调用现有求解和显示逻辑
        show(); // 来自solution_of_Suduko.h
    }
}
// File: ranking_system.h
#pragma once
#include <fstream>
#include <vector>
#include <algorithm>
#include "all_params.h"

struct GameRecord {
    string player;
    int timeSec;
    Difficulty::Level level;
};

namespace Ranking {
    vector<GameRecord> records;

    // 从文件加载记录
    void loadRecords() {
        ifstream file("ranking.dat", ios::binary);
        if (file) {
            GameRecord rec;
            while (file.read((char*)&rec, sizeof(rec))) {
                records.push_back(rec);
            }
        }
    }

    // 保存记录到文件
    void saveRecords() {
        ofstream file("ranking.dat", ios::binary);
        for (auto& rec : records) {
            file.write((char*)&rec, sizeof(rec));
        }
    }

    // 显示排名界面
    void showRanking() {
        // 实现排名显示图形逻辑...
    }
}
// 修改后的main_system.cpp
#include "login_ui.h"
#include "difficulty_select.h"
#include "game_ui.h"

int main() {
    if (LoginUI::showLogin()) {
        Difficulty::Level level = Difficulty::selectDifficulty();
        GameUI::startGame(level);
        Ranking::showRanking();
    }
    return 0;
}