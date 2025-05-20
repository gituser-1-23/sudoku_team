#pragma once
#include<iostream>
#include<vector>
#include<string>
#include"create_players.h"
using namespace std;

string folder_path0 = "./players/";
//���ȵ�һ������������Ҫ���д��ļ��������ȡ�����ǵ�������ҵ���Ϣ
//��������ʹ��������ģ������
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
                cout << "�ɹ�ѡ�����: " << nam << endl;
                return true;
            }
        }
        cout << "��� " << nam << " �����ڻ��������!" << endl;
        return false;
    }
    // ��ȡ��ǰ��ҵ�Player����ָ��
    Player* getCurrentPlayer()
    {
        if (now.empty())
        {
            cout << "����û��ѡ�е����!" << endl;
            return nullptr;
        }
        // ���ҵ�ǰ��ҵ���Ϣ
        for (const auto& info : plist)
        {
            if (info.name == now)
            {
                // ����һ����ʱPlayer���󲢷�����ָ��
                // ע�⣺����ڶ��ϴ����¶��󣬵�������Ҫ����ɾ��
                return new Player(info);
            }
        }
        cout << "�����޷��ҵ���ǰ��ҵ���Ϣ!" << endl;
        return nullptr;
    }
    vector<Player::PlayerInfo> getplist()
    {
        return plist;
    }
    // �������������ʹ��ԭʼָ��汾��Ҫע���ڴ����
    ~Player_list() = default;
};