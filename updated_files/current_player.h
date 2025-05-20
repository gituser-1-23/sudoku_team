#pragma once
#include<iostream>
#include<vector>
#include<string>
#include"create_players.h"
using namespace std;

string folder_path0 = "./players/";
//首先第一步就是我们需要进行从文件夹里面读取到我们的所有玩家的信息
//这里我们使用向量来模拟链表
vector<Player::PlayerInfo> LOAD(const string& folder_path = folder_path0)
{
	return Player::loadAllPlayerInfos(folder_path);
}

class Player_list
{
private:
	vector<Player::PlayerInfo> plist;
	string now;
public:
	Player_list() { plist = LOAD(); now = ""; }
    bool choose(const string& nam, const string& cod)
    {
        for (const auto& info : plist)
        {
            if (nam == info.name && cod == info.code)
            {
                now = info.name;
                cout << "成功选择玩家: " << nam << endl;
                return true;
            }
        }
        cout << "玩家 " << nam << " 不存在或密码错误!" << endl;
        return false;
    }
    // 获取当前玩家的Player对象指针
    Player* getCurrentPlayer()
    {
        if (now.empty())
        {
            cout << "错误：没有选中的玩家!" << endl;
            return nullptr;
        }
        // 查找当前玩家的信息
        for (const auto& info : plist)
        {
            if (info.name == now)
            {
                // 创建一个临时Player对象并返回其指针
                // 注意：这会在堆上创建新对象，调用者需要负责删除
                return new Player(info);
            }
        }
        cout << "错误：无法找到当前玩家的信息!" << endl;
        return nullptr;
    }
    vector<Player::PlayerInfo> getplist()
    {
        return plist;
    }
    // 析构函数（如果使用原始指针版本需要注意内存管理）
    ~Player_list() = default;
};