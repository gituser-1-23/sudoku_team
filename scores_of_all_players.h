#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "players_num.h" // 假设包含 getAllTxtFiles() 函数
#include"player_system.h"

using namespace std;

// 游戏模式枚举
enum class GameMode
{
    Simple = 0,
    Medium = 1,
    Hard = 2
};

struct PlayerInfo
{
    string name;       // 玩家姓名
    double simpleTime; // 简单模式完成时间
    double mediumTime; // 中等模式完成时间
    double hardTime;   // 困难模式完成时间
};

string Folder_Path = "./players/";

// 获取所有玩家信息
vector<PlayerInfo> get_all_info()
{
    vector<PlayerInfo> result;
    vector<Player::PlayerInfo> playerInfos = Player::loadAllPlayerInfos(Folder_Path);

    // 转换为外部 PlayerInfo 结构体
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

// 对玩家信息按指定游戏模式进行排序
void sort_the_players(vector<PlayerInfo>& PI, GameMode s)
{
    // 根据游戏模式选择相应的时间字段进行排序
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

    // 移除未完成该模式的玩家（成绩为默认值 999999.9）
    auto it = std::remove_if(PI.begin(), PI.end(), [s](const PlayerInfo& p) {
        double time = (s == GameMode::Simple) ? p.simpleTime :
            (s == GameMode::Medium) ? p.mediumTime : p.hardTime;
        return time >= 999999.0; // 使用常量值比较，避免浮点数误差
        });

    PI.erase(it, PI.end());
}