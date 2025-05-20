#pragma once
#include<iostream>
#include"current_player.h"
#include<string>
#include<vector>
#include"create_players.h"
#include<windows.h> // �����ļ�������Windowsƽ̨��
using namespace std;

string folder_path1 = "./players/";

class PlayerService
{
private:
    Player_list playerList;
    Player* currentPlayer;

    // ����ļ���������player*�ļ���Windowsƽ̨ʾ�����ɸ�����Ҫ��ӿ�ƽ̨���룩
    void clearPlayerFiles(const string& folderPath)
    {
#ifdef _WIN32
        string searchPath = folderPath + "\\player*";
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

        if (hFind != INVALID_HANDLE_VALUE) 
        {
            do
            {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
                {
                    string filePath = folderPath + "\\" + findData.cFileName;
                    DeleteFileA(filePath.c_str()); // ɾ���ļ�
                }
            } while (FindNextFileA(hFind, &findData) != 0);
            FindClose(hFind);
        }
#else
        // Linux/macOS�汾�ɲο�֮ǰ��ʵ��
#endif
    }
    // ȷ����������Ϊ9x9������ʱ���0��
    vector<vector<int>> ensureValidSuduko(const vector<vector<int>>& suduko) {
        vector<vector<int>> validSuduko(9, vector<int>(9, 0)); // ��ʼ��Ϊ9x9ȫ0����
        for (size_t i = 0; i < min(suduko.size(), validSuduko.size()); ++i) {
            for (size_t j = 0; j < min(suduko[i].size(), validSuduko[i].size()); ++j) {
                validSuduko[i][j] = suduko[i][j];
            }
        }
        return validSuduko;
    }
public:
    PlayerService() : currentPlayer(nullptr) {}

    ~PlayerService() 
    {
        if (currentPlayer) 
        {
            delete currentPlayer;
        }
    }

    // ��½����
    bool login(const std::string& name, const std::string& code)
    {
        if (playerList.choose(name, code))
        {
            currentPlayer = playerList.getCurrentPlayer();
            return true;
        }
        return false;
    }

    // ע�Ṧ��
    void registerPlayer(const std::string& name, const std::string& code)
    {
        Player newPlayer(name, code);
        newPlayer.saveToFile();
        std::cout << "�ɹ�ע�����: " << name << endl;
    }

    // ����ע�Ṧ��
    void quickRegister()
    {
        Player::createNewPlayer("./players/");
    }

    // �޸ĸ�����Ϣ����
    bool modifyPlayerInfo(const std::string& newName, const std::string& newCode)
    {
        if (currentPlayer) 
        {
            currentPlayer->reset(newName, newCode);
            currentPlayer->saveToFile();
            std::cout << "�ɹ��޸ĸ�����Ϣ" << std::endl;
            return true;
        }
        std::cout << "���ȵ�¼" << std::endl;
        return false;
    }

    // �˳���½����
    void logout() {
        if (currentPlayer) 
        {
            delete currentPlayer;
            currentPlayer = nullptr;
            std::cout << "�ɹ��˳���¼" << std::endl;
        }
        else {
            std::cout << "����δ��¼" << std::endl;
        }
    }

    // ��ȡ��ǰ���
    Player* getCurrentPlayer() {
        return currentPlayer;
    }

    // �������������Ϣ���ļ���
    void save_to_folder(const string& fdp = "./players/") {
        clearPlayerFiles(fdp); // ��վ��ļ�

        // ���±������������Ϣ
        for (const auto& info : playerList.getplist()) {
            Player player(info);

            // ȷ����������Ϊ9x9
            player.current_suduko = ensureValidSuduko(info.suduko);

            // д���ļ�
            ofstream file(fdp + player.getName() + ".txt");
            if (!file.is_open()) {
                cerr << "���棺�޷������ļ� " << player.getName() << ".txt" << endl;
                continue;
            }

            // д���������̣�ȷ��9�У�ÿ��9�����֣�
            file << "Suduko:\n";
            for (const auto& row : player.current_suduko) {
                for (int cell : row) {
                    file << cell << " ";
                }
                file << "\n"; // ȷ��ÿ�н����л���
            }
            file.close();

            // ����Player��saveToFile����������Ϣ���Ѱ���������
            player.saveToFile(fdp);
        }
        cout << "���������Ϣ���������棨��9x9�������̣�" << endl;
    }
};