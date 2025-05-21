#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm> // 用于 std::any_of
#include <cctype>    // 用于 isdigit
#include "players_num.h" // 假设包含 getmax_num() 声明
using namespace std;

class Player {
private:
    string name;            // 玩家名
    string code;            // 密码
    int simple_mode;        // 简单模式通关次数
    int mid_mode;           // 中等模式通关次数
    int hard_mode;          // 困难模式通关次数
    double current_time;    // 当前解棋盘已花费的时间
    double best_simple;     // 简单模式最佳成绩
    double best_mid;        // 中等模式最佳成绩
    double best_hard;       // 困难模式最佳成绩
    string current_mode;    // 当前难度选择

public:
    static const vector<vector<int>> empty1; // 默认空棋盘
    vector<vector<int>> current_suduko;      // 当前数独棋盘
    static int player_count;                 // 玩家数量统计

    // 构造函数（完整参数）
    Player(string name = "", string code = "123456", int simple = 0, int mid = 0, int hard = 0,
        const vector<vector<int>>& suduko = empty1, double ct = 0, string mod = "None")
        : name(name.empty() ? generateDefaultName() : name),
        code(code),
        simple_mode(simple),
        mid_mode(mid),
        hard_mode(hard),
        current_suduko(suduko),
        best_simple(999999.9),
        best_mid(999999.9),
        best_hard(999999.9),
        current_time(ct),
        current_mode(mod) {
        if (player_count >= INT_MAX) {
            throw runtime_error("玩家数量已达上限");
        }
        player_count++;
    }

    // 析构函数
    ~Player() = default;

    // 生成默认玩家名
    string generateDefaultName() const {
        ostringstream oss;
        oss << "player" << (player_count + 1);
        return oss.str();
    }

    // 玩家信息结构体
    struct PlayerInfo {
        string name;
        string code;
        int simpleMode;
        int midMode;
        int hardMode;
        vector<vector<int>> suduko;
        double bestSimple;
        double bestMid;
        double bestHard;
        double current_time;
        string current_mode;
    };

    // 结构体构造函数
    Player(const PlayerInfo& info)
        : name(info.name),
        code(info.code),
        simple_mode(info.simpleMode),
        mid_mode(info.midMode),
        hard_mode(info.hardMode),
        current_suduko(info.suduko),
        best_simple(info.bestSimple),
        best_mid(info.bestMid),
        best_hard(info.bestHard),
        current_time(info.current_time),
        current_mode(info.current_mode) {}

    // 获取玩家信息
    PlayerInfo getPlayerInfo() const {
        return {
            name, code, simple_mode, mid_mode, hard_mode,
            current_suduko, best_simple, best_mid, best_hard,
            current_time, current_mode
        };
    }

    // 从文件读取玩家信息到结构体
    static PlayerInfo loadPlayerInfoFromFile(const string& filePath) {
        PlayerInfo info;
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "错误：无法打开文件 " << filePath << endl;
            return info;
        }

        string line;
        bool readingSuduko = false;
        vector<vector<int>> sudukoBoard;

        while (getline(file, line)) {
            if (line == "Suduko:") {
                readingSuduko = true;
                continue;
            }

            size_t pos = line.find(':');
            if (pos == string::npos) {
                if (readingSuduko) {
                    // 处理数独行（无空格格式）
                    vector<int> row;
                    for (char c : line) {
                        if (isdigit(c)) {
                            row.push_back(c - '0');
                        }
                    }
                    // 补全9列
                    while (row.size() < 9) row.push_back(0);
                    sudukoBoard.push_back(row);
                }
                continue;
            }

            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            try {
                if (key == "Name") info.name = value;
                else if (key == "Code") info.code = value;
                else if (key == "SimpleMode") info.simpleMode = stoi(value);
                else if (key == "MidMode") info.midMode = stoi(value);
                else if (key == "HardMode") info.hardMode = stoi(value);
                else if (key == "BestSimple") info.bestSimple = stod(value);
                else if (key == "BestMid") info.bestMid = stod(value);
                else if (key == "BestHard") info.bestHard = stod(value);
                else if (key == "current_time") info.current_time = stod(value);
                else if (key == "current_mode") info.current_mode = value;
            }
            catch (const exception& e) {
                cerr << "错误：解析文件 " << filePath << " 失败，行：" << line << endl;
                file.close();
                return PlayerInfo();
            }
        }

        // 补全9行
        while (sudukoBoard.size() < 9) sudukoBoard.push_back(vector<int>(9, 0));
        info.suduko = sudukoBoard;
        file.close();
        return info;
    }

    // 加载所有玩家信息
    static vector<PlayerInfo> loadAllPlayerInfos(const string& folderPath = "./players/")
    {
        vector<PlayerInfo> allInfos;
        auto playerFiles = getAllTxtFiles(folderPath); // 假设此函数已实现
        for (const auto& fileName : playerFiles)
        {
            string filePath = folderPath + fileName;
            PlayerInfo info = loadPlayerInfoFromFile(filePath);
            if (!info.name.empty()) allInfos.push_back(info);
        }
        return allInfos;
    }

    // 设置难度模式
    void setmode(const string& mo)
    {
        current_mode = mo;
    }

    // 设置当前数独盘
    void setsuduko(const vector<vector<int>>& input)
    {
        current_suduko = input;
    }

    // 设置最佳成绩
    void setBestScore(double score)
    {
        string type = current_mode;
        if (type == "simple") {
            if (score < best_simple) {
                best_simple = score;
                resetSuduko();
            }
        }
        else if (type == "mid") {
            if (score < best_mid) {
                best_mid = score;
                resetSuduko();
            }
        }
        else if (type == "hard") {
            if (score < best_hard) {
                best_hard = score;
                resetSuduko();
            }
        }
        else {
            throw invalid_argument("无效的难度模式");
        }
    }

    // 保存玩家数据到文件（无空格数独格式）
    bool saveToFile(const string& folderPath = "./players/") const {
        // 创建文件夹
#ifdef _WIN32
        CreateDirectoryA(folderPath.c_str(), nullptr);
#else
        system(("mkdir -p \"" + folderPath + "\"").c_str());
#endif

        string filePath = folderPath + name + ".txt";
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "错误：无法创建文件 " << filePath << endl;
            return false;
        }

        // 写入键值对
        file << "Name:" << name << "\n";
        file << "Code:" << code << "\n";
        file << "SimpleMode:" << simple_mode << "\n";
        file << "MidMode:" << mid_mode << "\n";
        file << "HardMode:" << hard_mode << "\n";
        file << "BestSimple:" << best_simple << "\n";
        file << "BestMid:" << best_mid << "\n";
        file << "BestHard:" << best_hard << "\n";
        file << "current_time:" << current_time << "\n";
        file << "current_mode:" << current_mode << "\n";
        file << "Suduko:\n";

        // 写入数独棋盘（无空格，每行9数字）
        for (const auto& row : current_suduko) {
            for (int cell : row) {
                file << cell; // 直接拼接数字
            }
            file << "\n"; // 每行换行
        }

        file.close();
        return true;
    }

    // 从文件加载玩家数据
    bool loadFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "错误：无法打开文件 " << filePath << endl;
            return false;
        }

        string line;
        bool readingSuduko = false;
        current_suduko.clear();

        while (getline(file, line)) {
            if (line == "Suduko:") {
                readingSuduko = true;
                continue;
            }

            if (readingSuduko) {
                // 解析数独行（忽略非数字字符，补全9列）
                vector<int> row;
                for (char c : line) {
                    if (isdigit(c)) {
                        row.push_back(c - '0');
                    }
                }
                while (row.size() < 9) row.push_back(0); // 补全
                current_suduko.push_back(row);
            }
            else {
                // 解析键值对
                size_t pos = line.find(':');
                if (pos == string::npos) continue;

                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);

                try {
                    if (key == "Name") name = value;
                    else if (key == "Code") code = value;
                    else if (key == "SimpleMode") simple_mode = stoi(value);
                    else if (key == "MidMode") mid_mode = stoi(value);
                    else if (key == "HardMode") hard_mode = stoi(value);
                    else if (key == "BestSimple") best_simple = stod(value);
                    else if (key == "BestMid") best_mid = stod(value);
                    else if (key == "BestHard") best_hard = stod(value);
                    else if (key == "current_time") current_time = stod(value);
                    else if (key == "current_mode") current_mode = value;
                }
                catch (const exception& e) {
                    cerr << "错误：解析文件 " << filePath << " 失败，行：" << line << endl;
                    file.close();
                    return false;
                }
            }
        }

        // 补全9行
        while (current_suduko.size() < 9)
        {
            current_suduko.push_back(vector<int>(9, 0));
        }

        file.close();
        return true;
    }

    // 统计玩家数量
    static int getPlayerCount()
    {
        return player_count;
    }

    // 修改玩家信息
    void reset(string newName = "", string newCode = "") 
    {
        if (!newName.empty()) name = newName;
        if (!newCode.empty()) code = newCode;
    }

    // 更新解题时间
    void update_time(double additional) 
    {
        current_time += additional;
    }

    // Getter 方法
    string getName() const { return name; }
    string getCode() const { return code; }
    int getSimpleModeCount() const { return simple_mode; }
    int getMidModeCount() const { return mid_mode; }
    int getHardModeCount() const { return hard_mode; }
    vector<vector<int>> getCurrentSuduko() const { return current_suduko; }
    double getBestSimpleScore() const { return best_simple; }
    double getBestMidScore() const { return best_mid; }
    double getBestHardScore() const { return best_hard; }
    double getcurrent_time() const { return current_time; }
    string getcurrent_mode() const { return current_mode; }

    // 显示数独棋盘
    void showcurrent_suduko() const {
        for (const auto& row : current_suduko) {
            for (int cell : row) {
                cout << cell;
            }
            cout << endl;
        }
    }

    // 静态方法：创建新玩家
    static void createNewPlayer(const string& folderPath) {
        Player newPlayer;
        newPlayer.saveToFile(folderPath);
        cout << "成功创建玩家：" << newPlayer.getName() << endl;
    }

private:
    // 重置棋盘及状态（私有辅助函数）
    void resetSuduko() {
        current_suduko = empty1;
        current_time = 0;
        current_mode = "None";
        // 通关次数在 setBestScore 中已增加，此处无需处理
    }
};

// 静态成员初始化
int Player::player_count = getmax_num(); // 需包含 getmax_num() 定义
const vector<vector<int>> Player::empty1 = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

#endif // PLAYER_H