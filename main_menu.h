// main_menu.h
#pragma once
#include <graphics.h>
#include <conio.h>
#include <string>
#include "player_system.h"
#include "sort_the_players.h"
#include "solution_of_Suduko.h"

using namespace std;

// 主菜单选项枚举
enum MainMenuOption {
    OPTION_CHALLENGE = 1,
    OPTION_SCOREBOARD,
    OPTION_EXIT
};

// 绘制主菜单
void drawMainMenu() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(40, 0, _T("宋体"));
    outtextxy(100, 100, _T("数独游戏"));

    settextstyle(28, 0, _T("宋体"));
    outtextxy(150, 200, _T("1. 挑战"));
    outtextxy(150, 250, _T("2. 查看排名"));
    outtextxy(150, 300, _T("3. 退出"));

    setlinecolor(WHITE);
    rectangle(100, 180, 350, 230);
    rectangle(100, 230, 350, 280);
    rectangle(100, 280, 350, 330);
}

// 处理菜单输入
MainMenuOption handleMenuInput() {
    MOUSEMSG msg;
    while (true) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 挑战按钮区域 (100,180)-(350,230)
                if (msg.x >= 100 && msg.x <= 350 && msg.y >= 180 && msg.y <= 230) {
                    return OPTION_CHALLENGE;
                }
                // 排名按钮区域 (100,230)-(350,280)
                else if (msg.x >= 100 && msg.x <= 350 && msg.y >= 230 && msg.y <= 280) {
                    return OPTION_SCOREBOARD;
                }
                // 退出按钮区域 (100,280)-(350,330)
                else if (msg.x >= 100 && msg.x <= 350 && msg.y >= 280 && msg.y <= 330) {
                    return OPTION_EXIT;
                }
            }
        }
        // 处理键盘按下 ESC 键退出
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // ESC 键的 ASCII 码是 27
                return OPTION_EXIT;
            }
        }
    }
}

// 显示排名信息
// 显示排名信息
void showScoreboard() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(24, 0, _T("宋体"));

    vector<string> ranks = get_the_info();
    outtextxy(50, 50, _T("简单模式排名:"));
    if (!ranks.empty()) {
        std::wstring wstr = std::wstring(ranks[0].begin(), ranks[0].end());
        outtextxy(50, 80, wstr.c_str());
    }

    outtextxy(50, 180, _T("中等模式排名:"));
    if (ranks.size() > 1) {
        std::wstring wstr = std::wstring(ranks[1].begin(), ranks[1].end());
        outtextxy(50, 210, wstr.c_str());
    }

    outtextxy(50, 310, _T("困难模式排名:"));
    if (ranks.size() > 2) {
        std::wstring wstr = std::wstring(ranks[2].begin(), ranks[2].end());
        outtextxy(50, 340, wstr.c_str());
    }

    outtextxy(50, 450, _T("按任意键返回主菜单..."));
    while (!_kbhit());
}