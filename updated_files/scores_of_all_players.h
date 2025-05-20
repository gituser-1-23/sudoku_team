#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "players_num.h" // �������getAllTxtFiles()


using namespace std;

// ��Ϸģʽö��
enum class GameMode
{
    Simple = 0,
    Medium = 1,
    Hard = 2
};

struct PlayerInfo
{
    string name;       // �����
    double simpleTime;      // ��ģʽ���ʱ��
    double mediumTime;      // �е�ģʽ���ʱ��
    double hardTime;        // ����ģʽ���ʱ��
};

string Folder_Path = "./players/";
vector<PlayerInfo> get_all_info(string FP = Folder_Path)
{
    vector<PlayerInfo> result;

    // ��ȡ��������ļ�
    vector<string> playerFiles;
#ifdef _WIN32
    playerFiles = getAllTxtFiles(FP);
#else
    playerFiles = getAllTxtFiles(FP);
#endif

    // ���������ļ������������Ϣ
    for (const string& filename : playerFiles) {
        string filePath = FP + filename;

        // ���� Player ���󲢴��ļ���������
        Player player;
        if (player.loadFromFile(filePath)) {
            // �� Player �����ȡ��Ϣ��ת��Ϊ PlayerInfo �ṹ��
            Player::PlayerInfo playerInfo = player.getPlayerInfo();

            // ת��Ϊ�ⲿ����� PlayerInfo �ṹ��
            PlayerInfo info;
            info.name = playerInfo.name;
            info.simpleTime = playerInfo.bestSimple;
            info.mediumTime = playerInfo.bestMid;
            info.hardTime = playerInfo.bestHard;

            result.push_back(info);
        }
        else {
            cerr << "���棺�޷���������ļ� " << filePath << endl;
        }
    }

    return result;
}

void sort_the_players(vector<PlayerInfo>& PI, GameMode s)
{
    // ������Ϸģʽѡ���Ӧ��ʱ���ֶν�������
    switch (s) {
    case GameMode::Simple:
        std::sort(PI.begin(), PI.end(),
            [](const PlayerInfo& a, const PlayerInfo& b) {
                return a.simpleTime < b.simpleTime;
            });
        break;

    case GameMode::Medium:
        std::sort(PI.begin(), PI.end(),
            [](const PlayerInfo& a, const PlayerInfo& b) {
                return a.mediumTime < b.mediumTime;
            });
        break;

    case GameMode::Hard:
        std::sort(PI.begin(), PI.end(),
            [](const PlayerInfo& a, const PlayerInfo& b) {
                return a.hardTime < b.hardTime;
            });
        break;
    }

    // �Ƴ�δ��ɸ�ģʽ����ң��ɼ�ΪĬ��ֵ 999999.9��
    auto it = std::remove_if(PI.begin(), PI.end(), [s](const PlayerInfo& p) {
        double time = (s == GameMode::Simple) ? p.simpleTime :
            (s == GameMode::Medium) ? p.mediumTime : p.hardTime;
        return time >= 999999.0; // ʹ�ý���ֵ�Ƚϣ����⸡������������
        });

    PI.erase(it, PI.end());
}