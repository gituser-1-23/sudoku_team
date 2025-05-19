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

    // ��¼����������߼�
    bool showLogin() {
        initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
        bool loginSuccess = false;

        // ���������Ͱ�ť
        settextstyle(20, 0, _T("����"));
        outtextxy(LOGIN_BTN_X - 120, 150, _T("�û���:"));
        outtextxy(LOGIN_BTN_X - 120, 250, _T("����:"));

        // �����߼�
        MOUSEMSG msg;
        while (true) {
            msg = GetMouseMsg();
            bool loginHover = isButtonHovered(msg.x, msg.y, LOGIN_BTN_X, LOGIN_BTN_Y, LOGIN_BTN_WIDTH, LOGIN_BTN_HEIGHT);

            // ���ƶ�̬��ť
            drawButton(LOGIN_BTN_X, LOGIN_BTN_Y, LOGIN_BTN_WIDTH, LOGIN_BTN_HEIGHT, _T("��¼"), loginHover);

            if (msg.uMsg == WM_LBUTTONDOWN && loginHover) {
                loginSuccess = true; // ʵ��Ӧ��֤����
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

    // �Ѷ�ѡ�����
    Level selectDifficulty() {
        initgraph(400, 300);
        Level selected = EASY;

        // ��ť����
        const int BTN_WIDTH = 120;
        const int BTN_HEIGHT = 50;
        const int BTN_Y_START = 100;

        // ���ư�ť
        MOUSEMSG msg;
        while (true) {
            cleardevice();
            settextstyle(25, 0, _T("����"));
            outtextxy(150, 50, _T("ѡ���Ѷ�"));

            // �����ѶȰ�ť
            bool easyHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START, BTN_WIDTH, BTN_HEIGHT, _T("��"), easyHover);

            bool medHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START + 70, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START + 70, BTN_WIDTH, BTN_HEIGHT, _T("�е�"), medHover);

            bool hardHover = isButtonHovered(msg.x, msg.y, 140, BTN_Y_START + 140, BTN_WIDTH, BTN_HEIGHT);
            drawButton(140, BTN_Y_START + 140, BTN_WIDTH, BTN_HEIGHT, _T("����"), hardHover);

            // ������
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (easyHover) return EASY;
                if (medHover) return MEDIUM;
                if (hardHover) return HARD;
            }
            Sleep(10); // ����CPUռ��
        }
    }
}
// File: game_ui.h
#pragma once
#include "page_of_solve.h"
#include "solution_of_Suduko.h"
#include "ranking_system.h"

namespace GameUI {
    // ������Ϸ������
    void startGame(Difficulty::Level level) {
        // �����Ѷȼ��ز�ͬ�����ļ�
        string file = (level == Difficulty::EASY) ? "easy.txt" :
            (level == Difficulty::MEDIUM) ? "medium.txt" : "hard.txt";
        suduko_file = file; // ����ȫ���ļ���

        // ��������������ʾ�߼�
        show(); // ����solution_of_Suduko.h
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

    // ���ļ����ؼ�¼
    void loadRecords() {
        ifstream file("ranking.dat", ios::binary);
        if (file) {
            GameRecord rec;
            while (file.read((char*)&rec, sizeof(rec))) {
                records.push_back(rec);
            }
        }
    }

    // �����¼���ļ�
    void saveRecords() {
        ofstream file("ranking.dat", ios::binary);
        for (auto& rec : records) {
            file.write((char*)&rec, sizeof(rec));
        }
    }

    // ��ʾ��������
    void showRanking() {
        // ʵ��������ʾͼ���߼�...
    }
}
// �޸ĺ��main_system.cpp
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