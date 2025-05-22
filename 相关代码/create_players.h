#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm> // ���� std::any_of
#include <cctype>    // ���� isdigit
#include "players_num.h" // ������� getmax_num() ����
using namespace std;

class Player {
private:
    string name;            // �����
    string code;            // ����
    int simple_mode;        // ��ģʽͨ�ش���
    int mid_mode;           // �е�ģʽͨ�ش���
    int hard_mode;          // ����ģʽͨ�ش���
    double current_time;    // ��ǰ�������ѻ��ѵ�ʱ��
    double best_simple;     // ��ģʽ��ѳɼ�
    double best_mid;        // �е�ģʽ��ѳɼ�
    double best_hard;       // ����ģʽ��ѳɼ�
    string current_mode;    // ��ǰ�Ѷ�ѡ��

public:
    static const vector<vector<int>> empty1; // Ĭ�Ͽ�����
    vector<vector<int>> current_suduko;      // ��ǰ��������
    static int player_count;                 // �������ͳ��

    // ���캯��������������
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
            throw runtime_error("��������Ѵ�����");
        }
        player_count++;
    }

    // ��������
    ~Player() = default;

    // ����Ĭ�������
    string generateDefaultName() const {
        ostringstream oss;
        oss << "player" << (player_count + 1);
        return oss.str();
    }

    // �����Ϣ�ṹ��
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

    // �ṹ�幹�캯��
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

    // ��ȡ�����Ϣ
    PlayerInfo getPlayerInfo() const {
        return {
            name, code, simple_mode, mid_mode, hard_mode,
            current_suduko, best_simple, best_mid, best_hard,
            current_time, current_mode
        };
    }

    // ���ļ���ȡ�����Ϣ���ṹ��
    static PlayerInfo loadPlayerInfoFromFile(const string& filePath) {
        PlayerInfo info;
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "�����޷����ļ� " << filePath << endl;
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
                    // ���������У��޿ո��ʽ��
                    vector<int> row;
                    for (char c : line) {
                        if (isdigit(c)) {
                            row.push_back(c - '0');
                        }
                    }
                    // ��ȫ9��
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
                cerr << "���󣺽����ļ� " << filePath << " ʧ�ܣ��У�" << line << endl;
                file.close();
                return PlayerInfo();
            }
        }

        // ��ȫ9��
        while (sudukoBoard.size() < 9) sudukoBoard.push_back(vector<int>(9, 0));
        info.suduko = sudukoBoard;
        file.close();
        return info;
    }

    // �������������Ϣ
    static vector<PlayerInfo> loadAllPlayerInfos(const string& folderPath = "./players/")
    {
        vector<PlayerInfo> allInfos;
        auto playerFiles = getAllTxtFiles(folderPath); // ����˺�����ʵ��
        for (const auto& fileName : playerFiles)
        {
            string filePath = folderPath + fileName;
            PlayerInfo info = loadPlayerInfoFromFile(filePath);
            if (!info.name.empty()) allInfos.push_back(info);
        }
        return allInfos;
    }

    // �����Ѷ�ģʽ
    void setmode(const string& mo)
    {
        current_mode = mo;
    }

    // ���õ�ǰ������
    void setsuduko(const vector<vector<int>>& input)
    {
        current_suduko = input;
    }

    // ������ѳɼ�
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
            throw invalid_argument("��Ч���Ѷ�ģʽ");
        }
    }

    // ����������ݵ��ļ����޿ո�������ʽ��
    bool saveToFile(const string& folderPath = "./players/") const {
        // �����ļ���
#ifdef _WIN32
        CreateDirectoryA(folderPath.c_str(), nullptr);
#else
        system(("mkdir -p \"" + folderPath + "\"").c_str());
#endif

        string filePath = folderPath + name + ".txt";
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "�����޷������ļ� " << filePath << endl;
            return false;
        }

        // д���ֵ��
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

        // д���������̣��޿ո�ÿ��9���֣�
        for (const auto& row : current_suduko) {
            for (int cell : row) {
                file << cell; // ֱ��ƴ������
            }
            file << "\n"; // ÿ�л���
        }

        file.close();
        return true;
    }

    // ���ļ������������
    bool loadFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "�����޷����ļ� " << filePath << endl;
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
                // ���������У����Է������ַ�����ȫ9�У�
                vector<int> row;
                for (char c : line) {
                    if (isdigit(c)) {
                        row.push_back(c - '0');
                    }
                }
                while (row.size() < 9) row.push_back(0); // ��ȫ
                current_suduko.push_back(row);
            }
            else {
                // ������ֵ��
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
                    cerr << "���󣺽����ļ� " << filePath << " ʧ�ܣ��У�" << line << endl;
                    file.close();
                    return false;
                }
            }
        }

        // ��ȫ9��
        while (current_suduko.size() < 9)
        {
            current_suduko.push_back(vector<int>(9, 0));
        }

        file.close();
        return true;
    }

    // ͳ���������
    static int getPlayerCount()
    {
        return player_count;
    }

    // �޸������Ϣ
    void reset(string newName = "", string newCode = "") 
    {
        if (!newName.empty()) name = newName;
        if (!newCode.empty()) code = newCode;
    }

    // ���½���ʱ��
    void update_time(double additional) 
    {
        current_time += additional;
    }

    // Getter ����
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

    // ��ʾ��������
    void showcurrent_suduko() const {
        for (const auto& row : current_suduko) {
            for (int cell : row) {
                cout << cell;
            }
            cout << endl;
        }
    }

    // ��̬���������������
    static void createNewPlayer(const string& folderPath) {
        Player newPlayer;
        newPlayer.saveToFile(folderPath);
        cout << "�ɹ�������ң�" << newPlayer.getName() << endl;
    }

private:
    // �������̼�״̬��˽�и���������
    void resetSuduko() {
        current_suduko = empty1;
        current_time = 0;
        current_mode = "None";
        // ͨ�ش����� setBestScore �������ӣ��˴����账��
    }
};

// ��̬��Ա��ʼ��
int Player::player_count = getmax_num(); // ����� getmax_num() ����
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