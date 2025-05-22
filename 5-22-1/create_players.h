#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept> // �����쳣����

using namespace std;

class Player 
{
private:
    string name;            // �����
    string code;            // ����
    int simple_mode;        // ��ģʽͨ�ش���
    int mid_mode;           // �е�ģʽͨ�ش���
    int hard_mode;          // ����ģʽͨ�ش���
   
    
    static const vector<vector<int>> empty; // Ĭ�Ͽ�����

    double best_simple; // ��ģʽ��ѳɼ���ʱ��/������
    double best_mid;    // �е�ģʽ��ѳɼ�
    double best_hard;   // ����ģʽ��ѳɼ�

public:
    vector<vector<int>> current_suduko; // ��ǰ��������
    static int player_count;             // �������ͳ��
    // ���캯��
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
        if (player_count >= INT_MAX) { // ��ֹ�������
            throw runtime_error("��������Ѵ�����");
        }
        player_count++;
    }

    // ��������
    ~Player() = default;

    // �ڲ�����������Ĭ����������� player1, player2��
    string generateDefaultName() const 
    {
        ostringstream oss;
        oss << "player" << (player_count + 1);
        return oss.str();
    }

    // ��ȡ�����Ϣ�ṹ��
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
        // ע�⣺���ﲻ����player_count����Ϊ�ⲻ���´��������
        // ���Ǵ����е���Ϣ�������
    }
    // ��ȡ���������Ϣ
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
    //�����txt�ļ������ȡ��������Ϣ���ṹ��
    // �ӵ����ļ���ȡ�����Ϣ��PlayerInfo�ṹ��
    static PlayerInfo loadPlayerInfoFromFile(const string& filePath) {
        PlayerInfo info;
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "�����޷����ļ� " << filePath << endl;
            return info; // ����Ĭ�Ϲ����PlayerInfo
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
                cerr << "���󣺽����ļ� " << filePath << " ʧ�ܣ��У�" << line << endl;
                file.close();
                return PlayerInfo(); // ���ؿսṹ��
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
    // ���ļ����м������������Ϣ��vector<PlayerInfo>
    static vector<PlayerInfo> loadAllPlayerInfos(const string& folderPath = "./players/") {
        vector<PlayerInfo> allInfos;
        auto playerFiles = getAllTxtFiles(folderPath); // ʹ��֮ǰʵ�ֵĻ�ȡplayer*�ļ��б�ĺ���

        for (const auto& fileName : playerFiles) {
            string filePath = folderPath + fileName;
            PlayerInfo info = loadPlayerInfoFromFile(filePath);
            if (!info.name.empty()) { // ȷ����Ч����
                allInfos.push_back(info);
            }
        }
        return allInfos;
    }
    // ������ѳɼ� "simple","mid","hard"
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
        else throw invalid_argument("�ٽ�����Ŷ!");
    }

    // ����������ݵ��ļ�������������Ϣ��
    bool saveToFile(const string& folderPath = "./players/") const
    {
        // �����ļ��У���ƽ̨ʵ�֣�
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

        // д���ʽ����ֵ�� + ��������
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

    // ���ļ�����������ݣ�����������Ϣ��
    bool loadFromFile(const string& filePath)
    {
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
                cerr << "�����ļ���ʽ�����У�" << line << endl;
                file.close();
                return false;
            }
            catch (const out_of_range& e) {
                cerr << "������ֵ������Χ���У�" << line << endl;
                file.close();
                return false;
            }

            if (readingSuduko) {
                istringstream iss(value); // ֱ�ӽ���������
                vector<int> row;
                int cell;
                while (iss >> cell) row.push_back(cell);
                if (!row.empty()) current_suduko.push_back(row);
            }
        }
        file.close();
        return true;
    }  

    // ͳ�������������̬������
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

    // ���� Getter ����
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

// ��̬��Ա��ʼ��
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

// ��������Ҳ����浽�ļ�����̬������
inline void Player::createNewPlayer(const string& folderPath)
{
    Player newPlayer;
    newPlayer.saveToFile(folderPath);
    cout << "�ɹ�������ң�" << newPlayer.getName() << endl;
}

#endif // PLAYER_H