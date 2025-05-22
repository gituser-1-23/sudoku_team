#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"create_players.h"
#include"all_params.h"
#include<windows.h> // 文件操作依赖 Windows 平台
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
    // 清除文件夹中所有 player* 文件（Windows 平台实现）
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
                    DeleteFileA(filePath.c_str()); // 删除文件
                }
            } while (FindNextFileA(hFind, &findData) != 0);
            FindClose(hFind);
        }
#else
        // Linux/macOS 版本可参考之前的实现
#endif
    }
    // 确保数独棋盘为 9x9 矩阵，不足部分补 0
    vector<vector<int>> ensureValidSuduko(const vector<vector<int>>& suduko) {
        vector<vector<int>> validSuduko(9, vector<int>(9, 0)); // 初始化为 9x9 全 0 矩阵
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

    // 登录方法
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

    // 注册方法
    void registerPlayer( std::string& name, std::string& code)
    {
        Player newPlayer(name, code);
        newPlayer.saveToFile();
        std::cout << "成功注册玩家: " << name << endl;
        playerList = LOAD();
    }

    // 快速注册方法
    void quickRegister()
    {
        Player::createNewPlayer("./players/");
        playerList = LOAD();
    }

    // 修改玩家信息方法
    bool modifyPlayerInfo(std::string& newName,std::string& newCode)
    {
        if (currentPlayer)
        {
            currentPlayer->reset(newName, newCode);
            currentPlayer->saveToFile();
            std::cout << "成功修改玩家信息" << std::endl;
            playerList = LOAD();
            return true;
        }
        std::cout << "请先登录" << std::endl;
        return false;
    }

    // 获取当前玩家指针
    Player* getCurrentPlayer()
    {
        return currentPlayer;
    }


    // 将所有玩家信息保存到文件夹,整个程序退出的时候就必须运行这个
    void save_to_folder(const string& fdp = "./players/")
    {
        

        // 重新保存所有玩家信息
        playerList = LOAD();
        clearPlayerFiles(fdp); // 清除旧文件
        for (const auto& info : playerList) {
            Player player(info);

            // 确保数独棋盘为 9x9
            player.current_suduko = ensureValidSuduko(info.suduko);

            // 写入文件
            ofstream file(fdp + player.getName() + ".txt");
            if (!file.is_open()) {
                cerr << "错误：无法打开文件 " << player.getName() << ".txt" << endl;
                continue;
            }

            // 写入数独棋盘，确保 9 行，每行 9 个数字
            file << "Suduko:\n";
            for (const auto& row : player.current_suduko) {
                for (int cell : row) {
                    file << cell << " ";
                }
                file << "\n"; // 确保每行结束有换行符
            }
            file.close();

            // 调用 Player 的 saveToFile 方法保存其他信息
            player.saveToFile(fdp);
        }
        cout << "所有玩家信息已保存到文件夹（9x9 数独棋盘格式）" << endl;
    }
    void show()
    {
        for (int i = 0; i < playerList.size(); i++)
        {
            cout << playerList[i].name << endl;
        }
    }

    //检查和登陆的方法
    void check_and_login(string&name,string&password)
    {
        if (this->login(name, password))
        {
            Player* player1 = this->getCurrentPlayer();
            std::cout << "欢迎回来，" << player1->getName() << std::endl;
        }
        else
        {
            std::cout << "错误：未找到玩家或密码错误，请检查输入！" << std::endl;
            // 可以在此处添加注册新玩家的逻辑
        }
    }
    // 注销登录方法
    void check_and_logout()
    {
        if (currentPlayer)
        {
            this->currentPlayer->saveToFile();
            delete currentPlayer;
            currentPlayer = nullptr;
            std::cout << "成功注销登录" << std::endl;
        }
        else
        {
            std::cout << "您尚未登录" << std::endl;
        }
    }
    // 快速注册且登陆的方法
    void quickRegisterAndLogin()
    {
        quickRegister();
        int maxNum = getmax_num("./players/");
        std::string newPlayerName = "player" + std::to_string(maxNum);
        std::string defaultCode = "123456"; // 假设默认密码是123456

        if (login(newPlayerName, defaultCode))
        {
            std::cout << "欢迎新玩家 " << newPlayerName << " 登录成功！" << std::endl;
        }
        else
            std::cout << "快速注册并登录失败。" << std::endl;
    }
   
};


//下面是比较重要的接口实用：
