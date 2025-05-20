#pragma once
#include<iostream>
#include"current_player.h"
#include<string>
#include<vector>
#include"create_players.h"
#include<windows.h> // 用于文件操作（Windows平台）
using namespace std;

string folder_path1 = "./players/";

class PlayerService
{
private:
    Player_list playerList;
    Player* currentPlayer;

    // 清空文件夹中所有player*文件（Windows平台示例，可根据需要添加跨平台代码）
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
        // Linux/macOS版本可参考之前的实现
#endif
    }
    // 确保数独棋盘为9x9（不足时填充0）
    vector<vector<int>> ensureValidSuduko(const vector<vector<int>>& suduko) {
        vector<vector<int>> validSuduko(9, vector<int>(9, 0)); // 初始化为9x9全0矩阵
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

    // 登陆功能
    bool login(const std::string& name, const std::string& code)
    {
        if (playerList.choose(name, code))
        {
            currentPlayer = playerList.getCurrentPlayer();
            return true;
        }
        return false;
    }

    // 注册功能
    void registerPlayer(const std::string& name, const std::string& code)
    {
        Player newPlayer(name, code);
        newPlayer.saveToFile();
        std::cout << "成功注册玩家: " << name << endl;
    }

    // 快速注册功能
    void quickRegister()
    {
        Player::createNewPlayer("./players/");
    }

    // 修改个人信息功能
    bool modifyPlayerInfo(const std::string& newName, const std::string& newCode)
    {
        if (currentPlayer) 
        {
            currentPlayer->reset(newName, newCode);
            currentPlayer->saveToFile();
            std::cout << "成功修改个人信息" << std::endl;
            return true;
        }
        std::cout << "请先登录" << std::endl;
        return false;
    }

    // 退出登陆功能
    void logout() {
        if (currentPlayer) 
        {
            delete currentPlayer;
            currentPlayer = nullptr;
            std::cout << "成功退出登录" << std::endl;
        }
        else {
            std::cout << "你尚未登录" << std::endl;
        }
    }

    // 获取当前玩家
    Player* getCurrentPlayer() {
        return currentPlayer;
    }

    // 完整保存玩家信息到文件夹
    void save_to_folder(const string& fdp = "./players/") {
        clearPlayerFiles(fdp); // 清空旧文件

        // 重新保存所有玩家信息
        for (const auto& info : playerList.getplist()) {
            Player player(info);

            // 确保数独棋盘为9x9
            player.current_suduko = ensureValidSuduko(info.suduko);

            // 写入文件
            ofstream file(fdp + player.getName() + ".txt");
            if (!file.is_open()) {
                cerr << "警告：无法创建文件 " << player.getName() << ".txt" << endl;
                continue;
            }

            // 写入数独棋盘（确保9行，每行9个数字）
            file << "Suduko:\n";
            for (const auto& row : player.current_suduko) {
                for (int cell : row) {
                    file << cell << " ";
                }
                file << "\n"; // 确保每行结束有换行
            }
            file.close();

            // 调用Player的saveToFile保存其他信息（已包含数独）
            player.saveToFile(fdp);
        }
        cout << "所有玩家信息已完整保存（含9x9数独棋盘）" << endl;
    }
};