#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "players_num.h" // ������� getAllTxtFiles() ����
#include"player_system.h"

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
    string name;       // �������
    double simpleTime; // ��ģʽ���ʱ��
    double mediumTime; // �е�ģʽ���ʱ��
    double hardTime;   // ����ģʽ���ʱ��
};

string Folder_Path = "./players/";

// ��ȡ���������Ϣ
vector<PlayerInfo> get_all_info()
{
    vector<PlayerInfo> result;
    vector<Player::PlayerInfo> playerInfos = Player::loadAllPlayerInfos(Folder_Path);

    // ת��Ϊ�ⲿ PlayerInfo �ṹ��
    for (const auto& info : playerInfos)
    {
        PlayerInfo externalInfo;
        externalInfo.name = info.name;
        externalInfo.simpleTime = info.bestSimple;
        externalInfo.mediumTime = info.bestMid;
        externalInfo.hardTime = info.bestHard;

        result.push_back(externalInfo);
    }

    return result;
}

// �������Ϣ��ָ����Ϸģʽ��������
void sort_the_players(vector<PlayerInfo>& PI, GameMode s)
{
    // ������Ϸģʽѡ����Ӧ��ʱ���ֶν�������
    switch (s)
    {
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
        return time >= 999999.0; // ʹ�ó���ֵ�Ƚϣ����⸡�������
        });

    PI.erase(it, PI.end());
}