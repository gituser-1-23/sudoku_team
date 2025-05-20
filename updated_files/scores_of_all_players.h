#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "players_num.h" // 假设包含getAllTxtFiles()


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
    string name;       // 玩家名
    double simpleTime;      // 简单模式最佳时间
    double mediumTime;      // 中等模式最佳时间
    double hardTime;        // 困难模式最佳时间
};

string Folder_Path = "./players/";
vector<PlayerInfo> get_all_info(string FP = Folder_Path)
{
    vector<PlayerInfo> result;

    // 获取所有玩家文件
    vector<string> playerFiles;
#ifdef _WIN32
    playerFiles = getAllTxtFiles(FP);
#else
    playerFiles = getAllTxtFiles(FP);
#endif

    // 遍历所有文件，加载玩家信息
    for (const string& filename : playerFiles) {
        string filePath = FP + filename;

        // 创建 Player 对象并从文件加载数据
        Player player;
        if (player.loadFromFile(filePath)) {
            // 从 Player 对象获取信息并转换为 PlayerInfo 结构体
            Player::PlayerInfo playerInfo = player.getPlayerInfo();

            // 转换为外部定义的 PlayerInfo 结构体
            PlayerInfo info;
            info.name = playerInfo.name;
            info.simpleTime = playerInfo.bestSimple;
            info.mediumTime = playerInfo.bestMid;
            info.hardTime = playerInfo.bestHard;

            result.push_back(info);
        }
        else {
            cerr << "警告：无法加载玩家文件 " << filePath << endl;
        }
    }

    return result;
}

void sort_the_players(vector<PlayerInfo>& PI, GameMode s)
{
    // 根据游戏模式选择对应的时间字段进行排序
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

    // 移除未完成该模式的玩家（成绩为默认值 999999.9）
    auto it = std::remove_if(PI.begin(), PI.end(), [s](const PlayerInfo& p) {
        double time = (s == GameMode::Simple) ? p.simpleTime :
            (s == GameMode::Medium) ? p.mediumTime : p.hardTime;
        return time >= 999999.0; // 使用近似值比较，避免浮点数精度问题
        });

    PI.erase(it, PI.end());
}