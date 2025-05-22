// main_menu.h
#pragma once
#include <graphics.h>
#include <conio.h>
#include <string>
#include "player_system.h"
#include "sort_the_players.h"
#include "solution_of_Suduko.h"

using namespace std;

// ���˵�ѡ��ö��
enum MainMenuOption {
    OPTION_CHALLENGE = 1,
    OPTION_SCOREBOARD,
    OPTION_EXIT
};

// �������˵�
void drawMainMenu() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(40, 0, _T("����"));
    outtextxy(100, 100, _T("������Ϸ"));

    settextstyle(28, 0, _T("����"));
    outtextxy(150, 200, _T("1. ��ս"));
    outtextxy(150, 250, _T("2. �鿴����"));
    outtextxy(150, 300, _T("3. �˳�"));

    setlinecolor(WHITE);
    rectangle(100, 180, 350, 230);
    rectangle(100, 230, 350, 280);
    rectangle(100, 280, 350, 330);
}

// ����˵�����
MainMenuOption handleMenuInput() {
    MOUSEMSG msg;
    while (true) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // ��ս��ť���� (100,180)-(350,230)
                if (msg.x >= 100 && msg.x <= 350 && msg.y >= 180 && msg.y <= 230) {
                    return OPTION_CHALLENGE;
                }
                // ������ť���� (100,230)-(350,280)
                else if (msg.x >= 100 && msg.x <= 350 && msg.y >= 230 && msg.y <= 280) {
                    return OPTION_SCOREBOARD;
                }
                // �˳���ť���� (100,280)-(350,330)
                else if (msg.x >= 100 && msg.x <= 350 && msg.y >= 280 && msg.y <= 330) {
                    return OPTION_EXIT;
                }
            }
        }
        // ������̰��� ESC ���˳�
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // ESC ���� ASCII ���� 27
                return OPTION_EXIT;
            }
        }
    }
}

// ��ʾ������Ϣ
// ��ʾ������Ϣ
void showScoreboard() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(24, 0, _T("����"));

    vector<string> ranks = get_the_info();
    outtextxy(50, 50, _T("��ģʽ����:"));
    if (!ranks.empty()) {
        std::wstring wstr = std::wstring(ranks[0].begin(), ranks[0].end());
        outtextxy(50, 80, wstr.c_str());
    }

    outtextxy(50, 180, _T("�е�ģʽ����:"));
    if (ranks.size() > 1) {
        std::wstring wstr = std::wstring(ranks[1].begin(), ranks[1].end());
        outtextxy(50, 210, wstr.c_str());
    }

    outtextxy(50, 310, _T("����ģʽ����:"));
    if (ranks.size() > 2) {
        std::wstring wstr = std::wstring(ranks[2].begin(), ranks[2].end());
        outtextxy(50, 340, wstr.c_str());
    }

    outtextxy(50, 450, _T("��������������˵�..."));
    while (!_kbhit());
}