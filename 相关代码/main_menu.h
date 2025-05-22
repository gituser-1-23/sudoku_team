// main_menu.h
#pragma once
#include <graphics.h>
#include <conio.h>
#include <string>
#include "player_system.h"
#include "sort_the_players.h"
#include "solution_of_Suduko.h"
#include <Windows.h>
#include <string>

using namespace std;

// ���˵�ѡ��ö��
enum MainMenuOption {
    OPTION_CHALLENGE = 1,
    OPTION_SCOREBOARD,
    OPTION_EXIT,
    OPTION_ACCOUNT
};

// �������˵�
void drawMainMenu() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(40, 0, _T("����"));
    outtextxy(100, 100, _T("������Ϸ"));

    settextstyle(28, 0, _T("����"));
    // ʹ�� _T() �꽫�ַ���ת��Ϊ LPCTSTR ����
    outtextxy(150, 200, _T("1. ��ս"));
    outtextxy(150, 250, _T("2. �鿴����"));
    outtextxy(150, 300, _T("3. �˳�"));

    setlinecolor(WHITE);
    rectangle(100, 180, 350, 230);
    rectangle(100, 230, 350, 280);
    rectangle(100, 280, 350, 330);

    // ���Ƹ����˻���ť
    int accountButtonX = 400;
    int accountButtonY = 20;
    int accountButtonWidth = 80;
    int accountButtonHeight = 30;
    setfillcolor(RGB(92, 92, 92));
    fillrectangle(accountButtonX, accountButtonY, accountButtonX + accountButtonWidth, accountButtonY + accountButtonHeight);
    settextcolor(RGB(255, 255, 255));
    settextstyle(18, 0, _T("����"));
    outtextxy(accountButtonX + 5, accountButtonY + 5, _T("�����˻�"));
}
// �������˵�����
MainMenuOption handleMenuInput() {
    MOUSEMSG msg;
    int accountButtonX = 400;
    int accountButtonY = 20;
    int accountButtonWidth = 80;
    int accountButtonHeight = 30;

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
                // �����˻���ť����
                else if (msg.x >= accountButtonX && msg.x <= accountButtonX + accountButtonWidth && msg.y >= accountButtonY && msg.y <= accountButtonY + accountButtonHeight) {
                    return OPTION_ACCOUNT;
                }
            }
            else if (msg.uMsg == WM_KEYDOWN) {
                // ʹ�� GetAsyncKeyState ��鰴��״̬
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    return OPTION_EXIT;
                }
            }
        }
    }
}
// �� std::string ת��Ϊ std::wstring
std::wstring stringToWstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// ��ʾ������
void showScoreboard() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(24, 0, _T("����"));

    vector<string> ranks = get_the_info();
    outtextxy(50, 50, _T("��ģʽ����:"));
    std::wstring wstr = stringToWstring(ranks[0]);
    outtextxy(50, 80, wstr.c_str());

    outtextxy(50, 180, _T("�е�ģʽ����:"));
    wstr = stringToWstring(ranks[1]);
    outtextxy(50, 210, wstr.c_str());

    outtextxy(50, 310, _T("����ģʽ����:"));
    wstr = stringToWstring(ranks[2]);
    outtextxy(50, 340, wstr.c_str());

    outtextxy(50, 450, _T("��������������˵�..."));
    while (!_kbhit());
}