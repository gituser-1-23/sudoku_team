#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<windows.h>

using namespace std;

#ifdef _WIN32
// ��ȡ����player*�ļ�
vector<string> getPlayerNames(const string& directory) {
    vector<string> playerNames;
    string searchPath = directory + "\\player*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                playerNames.push_back(findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }

    return playerNames;
}

// ��ȡ����txt�ļ�
vector<string> getAllTxtFiles(const string& directory) {
    vector<string> txtFiles;
    string searchPath = directory + "\\*.txt";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                txtFiles.push_back(findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }

    return txtFiles;
}
#else
// ��ȡ����player*�ļ�
vector<string> getPlayerNames(const string& directory) {
    vector<string> playerNames;
    DIR* dir = opendir(directory.c_str());

    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {  // ��ͨ�ļ�
                string fileName = entry->d_name;
                if (fileName.find("player") == 0) {
                    playerNames.push_back(fileName);
                }
            }
        }
        closedir(dir);
    }

    return playerNames;
}

// ��ȡ����txt�ļ�
vector<string> getAllTxtFiles(const string& directory)
{
    vector<string> txtFiles;
    DIR* dir = opendir(directory.c_str());

    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            if (entry->d_type == DT_REG) {  // ��ͨ�ļ�
                string fileName = entry->d_name;
                if (fileName.find(".txt") != string::npos)
                {
                    txtFiles.push_back(fileName);
                }
            }
        }
        closedir(dir);
    }

    return txtFiles;
}
#endif

// ��ȡplayer*�ļ���Ŀ
int number_of_players(string folder_path = "./players/") {
    return getPlayerNames(folder_path).size();
}

// ��ȡ����txt�ļ���Ŀ
int number_of_txt_files(string folder_path = "./players/") {
    return getAllTxtFiles(folder_path).size();
}