#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"create_players.h"
#include"all_params.h"
#include<windows.h> // �ļ��������� Windows ƽ̨
using namespace std;

string folder_path1 = folder_path;

vector<Player::PlayerInfo> LOAD(const string& folder_path = folder_path1)
{
    return Player::loadAllPlayerInfos(folder_path);
};

class PlayerService
{
private:
    Player* currentPlayer;
    vector<Player::PlayerInfo> playerList;
    // ����ļ��������� player* �ļ���Windows ƽ̨ʵ�֣�
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
        // Linux/macOS �汾�ɲο�֮ǰ��ʵ��
#endif
    }
    // ȷ����������Ϊ 9x9 ���󣬲��㲿�ֲ� 0
    vector<vector<int>> ensureValidSuduko(const vector<vector<int>>& suduko) {
        vector<vector<int>> validSuduko(9, vector<int>(9, 0)); // ��ʼ��Ϊ 9x9 ȫ 0 ����
        for (size_t i = 0; i < min(suduko.size(), validSuduko.size()); ++i) {
            for (size_t j = 0; j < min(suduko[i].size(), validSuduko[i].size()); ++j) {
                validSuduko[i][j] = suduko[i][j];
            }
        }
        return validSuduko;
    }
public:
    PlayerService() : currentPlayer(nullptr) { playerList = LOAD(); }
    ~PlayerService()
    {
        if (currentPlayer)
        {
            delete currentPlayer;
        }
    }

    // ��¼����
    bool login( std::string& name, std::string& code)
    {
        playerList = LOAD();
        for (const auto& info : playerList) 
        {
            if (info.name == name && info.code == code) 
            {
                currentPlayer = new Player(info);
                return true;
            }
        }
        return false;
    }

    // ע�᷽��
    void registerPlayer( std::string& name, std::string& code)
    {
        Player newPlayer(name, code);
        newPlayer.saveToFile();
        std::cout << "�ɹ�ע�����: " << name << endl;
        playerList = LOAD();
    }

    // ����ע�᷽��
    void quickRegister()
    {
        Player::createNewPlayer("./players/");
        playerList = LOAD();
    }

    // �޸������Ϣ����
    bool modifyPlayerInfo(std::string& newName,std::string& newCode)
    {
        if (currentPlayer)
        {
            currentPlayer->reset(newName, newCode);
            currentPlayer->saveToFile();
            std::cout << "�ɹ��޸������Ϣ" << std::endl;
            playerList = LOAD();
            return true;
        }
        std::cout << "���ȵ�¼" << std::endl;
        return false;
    }

    // ��ȡ��ǰ���ָ��
    Player* getCurrentPlayer()
    {
        return currentPlayer;
    }


    // �����������Ϣ���浽�ļ���,���������˳���ʱ��ͱ����������
    void save_to_folder(const string& fdp = "./players/")
    {
        

        // ���±������������Ϣ
        playerList = LOAD();
        clearPlayerFiles(fdp); // ������ļ�
        for (const auto& info : playerList) {
            Player player(info);

            // ȷ����������Ϊ 9x9
            player.current_suduko = ensureValidSuduko(info.suduko);

            // д���ļ�
            ofstream file(fdp + player.getName() + ".txt");
            if (!file.is_open()) {
                cerr << "�����޷����ļ� " << player.getName() << ".txt" << endl;
                continue;
            }

            // д���������̣�ȷ�� 9 �У�ÿ�� 9 ������
            file << "Suduko:\n";
            for (const auto& row : player.current_suduko) {
                for (int cell : row) {
                    file << cell << " ";
                }
                file << "\n"; // ȷ��ÿ�н����л��з�
            }
            file.close();

            // ���� Player �� saveToFile ��������������Ϣ
            player.saveToFile(fdp);
        }
        cout << "���������Ϣ�ѱ��浽�ļ��У�9x9 �������̸�ʽ��" << endl;
    }
    void show()
    {
        for (int i = 0; i < playerList.size(); i++)
        {
            cout << playerList[i].name << endl;
        }
    }

    //���͵�½�ķ���
    void check_and_login(string&name,string&password)
    {
        if (this->login(name, password))
        {
            Player* player1 = this->getCurrentPlayer();
            std::cout << "��ӭ������" << player1->getName() << std::endl;
        }
        else
        {
            std::cout << "����δ�ҵ���һ���������������룡" << std::endl;
            // �����ڴ˴����ע������ҵ��߼�
        }
    }
    // ע����¼����
    void check_and_logout()
    {
        if (currentPlayer)
        {
            this->currentPlayer->saveToFile();
            delete currentPlayer;
            currentPlayer = nullptr;
            std::cout << "�ɹ�ע����¼" << std::endl;
        }
        else
        {
            std::cout << "����δ��¼" << std::endl;
        }
    }
    // ����ע���ҵ�½�ķ���
    void quickRegisterAndLogin()
    {
        quickRegister();
        int maxNum = getmax_num("./players/");
        std::string newPlayerName = "player" + std::to_string(maxNum);
        std::string defaultCode = "123456"; // ����Ĭ��������123456

        if (login(newPlayerName, defaultCode))
        {
            std::cout << "��ӭ����� " << newPlayerName << " ��¼�ɹ���" << std::endl;
        }
        else
            std::cout << "����ע�Ტ��¼ʧ�ܡ�" << std::endl;
    }
   
};


//�����ǱȽ���Ҫ�Ľӿ�ʵ�ã�
