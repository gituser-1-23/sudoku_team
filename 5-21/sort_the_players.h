#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"scores_of_all_players.h"
#include"player_system.h"
using namespace std;

vector<string> get_the_info()
{
    vector<string> playerFiles;
    vector<PlayerInfo> p_list_simple = get_all_info();
    vector<PlayerInfo> p_list_mid = get_all_info();
    vector<PlayerInfo> p_list_hard = get_all_info();

    // �Բ�ͬģʽ�������Ϣ��������
    sort_the_players(p_list_simple, GameMode::Simple);
    sort_the_players(p_list_mid, GameMode::Medium);
    sort_the_players(p_list_hard, GameMode::Hard);

    string simple_mode_rank = "";
    string mid_mode_rank = "";
    string hard_mode_rank = "";

    // ���ɼ�ģʽ�����ַ���
    for (int i = 0; i < p_list_simple.size(); i++)
    {
        simple_mode_rank += to_string(i + 1) + ". " + p_list_simple[i].name + ": " + to_string(p_list_simple[i].simpleTime) + '\n';
    }

    // �����е�ģʽ�����ַ���
    for (int i = 0; i < p_list_mid.size(); i++)
    {
        mid_mode_rank += to_string(i + 1) + ". " + p_list_mid[i].name + ": " + to_string(p_list_mid[i].mediumTime) + '\n';
    }

    // ��������ģʽ�����ַ���
    for (int i = 0; i < p_list_hard.size(); i++)
    {
        hard_mode_rank += to_string(i + 1) + ". " + p_list_hard[i].name + ": " + to_string(p_list_hard[i].hardTime) + '\n';
    }

    playerFiles.push_back(simple_mode_rank);
    playerFiles.push_back(mid_mode_rank);
    playerFiles.push_back(hard_mode_rank);
    return playerFiles;
}