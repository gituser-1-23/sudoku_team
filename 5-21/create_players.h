#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept> // 用于异常处理

using namespace std;

class Player 
{
private:
    string name;            // 玩家名
    string code;            // 密码
    int simple_mode;        // 简单模式通关次数
    int mid_mode;           // 中等模式通关次数
    int hard_mode;          // 困难模式通关次数
   
    
    static const vector<vector<int>> empty; // 默认空棋盘

    double best_simple; // 简单模式最佳成绩（时间/分数）
    double best_mid;    // 中等模式最佳成绩
    double best_hard;   // 困难模式最佳成绩

public:
    vector<vector<int>> current_suduko; // 当前数独棋盘
    static int player_count;             // 玩家数量统计
    // 构造函数
    Player(string name = "", string code = "123456", int simple = 0, int mid = 0, int hard = 0,
        const vector<vector<int>>& suduko = empty)
        : name(name.empty() ? generateDefaultName() : name),
        code(code),
        simple_mode(simple),
        mid_mode(mid),
        hard_mode(hard),
        current_suduko(suduko),
        best_simple(999999.9),
        best_mid(999999.9),
        best_hard(999999.9) {
        if (player_count >= INT_MAX) { // 防止整数溢出
            throw runtime_error("玩家数量已达上限");
        }
        player_count++;
    }

    // 析构函数
    ~Player() = default;

    // 内部方法：生成默认玩家名（如 player1, player2）
    string generateDefaultName() const 
    {
        ostringstream oss;
        oss << "player" << (player_count + 1);
        return oss.str();
    }

    // 获取玩家信息结构体
    struct PlayerInfo
    {
        string name;
        string code;
        int simpleMode;
        int midMode;
        int hardMode;
        vector<vector<int>> suduko;
        double bestSimple;
        double bestMid;
        double bestHard;
    };
    Player(const PlayerInfo& info)
        : name(info.name),
        code(info.code),
        simple_mode(info.simpleMode),
        mid_mode(info.midMode),
        hard_mode(info.hardMode),
        current_suduko(info.suduko),
        best_simple(info.bestSimple),
        best_mid(info.bestMid),
        best_hard(info.bestHard)
    {
        // 注意：这里不增加player_count，因为这不是新创建的玩家
        // 而是从已有的信息构造对象
    }
    // 获取玩家完整信息
    PlayerInfo getPlayerInfo() const
    {
        return 
        {
            name,
            code,
            simple_mode,
            mid_mode,
            hard_mode,
            current_suduko,
            best_simple,
            best_mid,
            best_hard
        };
    }
    //从玩家txt文件里面读取到所有信息到结构体
    // 从单个文件读取玩家信息到PlayerInfo结构体
    static PlayerInfo loadPlayerInfoFromFile(const string& filePath) {
        PlayerInfo info;
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "错误：无法打开文件 " << filePath << endl;
            return info; // 返回默认构造的PlayerInfo
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
            if (pos == string::npos) continue;

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
            }
            catch (const exception& e) {
                cerr << "错误：解析文件 " << filePath << " 失败，行：" << line << endl;
                file.close();
                return PlayerInfo(); // 返回空结构体
            }

            if (readingSuduko) {
                istringstream iss(value);
                vector<int> row;
                int cell;
                while (iss >> cell) row.push_back(cell);
                if (!row.empty()) sudukoBoard.push_back(row);
            }
        }
        info.suduko = sudukoBoard;
        file.close();
        return info;
    }
    // 从文件夹中加载所有玩家信息到vector<PlayerInfo>
    static vector<PlayerInfo> loadAllPlayerInfos(const string& folderPath = "./players/") {
        vector<PlayerInfo> allInfos;
        auto playerFiles = getAllTxtFiles(folderPath); // 使用之前实现的获取player*文件列表的函数

        for (const auto& fileName : playerFiles) {
            string filePath = folderPath + fileName;
            PlayerInfo info = loadPlayerInfoFromFile(filePath);
            if (!info.name.empty()) { // 确保有效数据
                allInfos.push_back(info);
            }
        }
        return allInfos;
    }
    // 设置最佳成绩 "simple","mid","hard"
    void setBestScore(double score,const string& type) 
    {
        if (type == "simple")
        {
            if (score<best_simple) 
			{
				best_simple = score;
                current_suduko = empty;
                simple_mode++;
			}
        }
        else if (type == "mid")
        {
            if (score < best_mid)
            {
                current_suduko = empty;
                best_mid = score;
                mid_mode++;
            }
        }
        else if (type == "hard") 
        {
			if (score < best_hard)
			{
                current_suduko = empty;
				best_hard = score;
                hard_mode++;
			}
		} 
        else throw invalid_argument("再接再励哦!");
    }

    // 保存玩家数据到文件（包含所有信息）
    bool saveToFile(const string& folderPath = "./players/") const
    {
        // 创建文件夹（跨平台实现）
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

        // 写入格式：键值对 + 数独棋盘
        file << "Name:" << name << "\n";
        file << "Code:" << code << "\n";
        file << "SimpleMode:" << simple_mode << "\n";
        file << "MidMode:" << mid_mode << "\n";
        file << "HardMode:" << hard_mode << "\n";
        file << "BestSimple:" << best_simple << "\n";
        file << "BestMid:" << best_mid << "\n";
        file << "BestHard:" << best_hard << "\n";
        file << "Suduko:\n";
        for (const auto& row : current_suduko) {
            for (int cell : row) {
                file << cell << " ";
            }
            file << "\n";
        }
        file.close();
        return true;
    }

    // 从文件加载玩家数据（包含所有信息）
    bool loadFromFile(const string& filePath)
    {
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
            }
            catch (const invalid_argument& e) {
                cerr << "错误：文件格式错误，行：" << line << endl;
                file.close();
                return false;
            }
            catch (const out_of_range& e) {
                cerr << "错误：数值超出范围，行：" << line << endl;
                file.close();
                return false;
            }

            if (readingSuduko) {
                istringstream iss(value); // 直接解析行内容
                vector<int> row;
                int cell;
                while (iss >> cell) row.push_back(cell);
                if (!row.empty()) current_suduko.push_back(row);
            }
        }
        file.close();
        return true;
    }  

    // 统计玩家数量（静态方法）
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

    // 其他 Getter 方法
    string getName() const { return name; }
    string getCode() const { return code; }
    int getSimpleModeCount() const { return simple_mode; }
    int getMidModeCount() const { return mid_mode; }
    int getHardModeCount() const { return hard_mode; }
    vector<vector<int>> getCurrentSuduko() const { return current_suduko; }
    double getBestSimpleScore() const { return best_simple; }
    double getBestMidScore() const { return best_mid; }
    double getBestHardScore() const { return best_hard; }



    static void createNewPlayer(const string& folderPath);
};

// 静态成员初始化
int Player::player_count = 0;
const vector<vector<int>> Player::empty = 
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// 创建新玩家并保存到文件（静态方法）
inline void Player::createNewPlayer(const string& folderPath)
{
    Player newPlayer;
    newPlayer.saveToFile(folderPath);
    cout << "成功创建玩家：" << newPlayer.getName() << endl;
}

#endif // PLAYER_H