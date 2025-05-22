#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <unordered_map>
#include <algorithm>
#pragma comment(lib, "winmm.lib")

using namespace std;

// ������������
struct PlayerConfig
{
    string name;
    string playlistFile;
    string singleMusicFile;
    bool loop = true;
    bool singleLoop = false;

    PlayerConfig(const string& n, const string& plFile = "")
        : name(n), playlistFile(plFile) {}
};

class AudioPlayer
{
private:
    string playerName;          // ����������
    vector<string> playlist;    // �赥

    int currentIndex = -1;      // ��ǰ��������
    bool isPlaying = false;     // ����״̬
    bool isPaused = false;      // ��ͣ״̬
    bool isLooping = true;      // ѭ��ģʽ
    bool isSingleLoop = false;  // ����ѭ����־
    atomic<bool> threadRunning{ false }; // �߳����б�־
    thread audioThread;         // ��Ƶ�߳�
    mutex controlMutex;         // ������
    condition_variable cmdCV;   // ������������
    enum Command { NONE, PLAY, PAUSE, RESUME, STOP, REPLAY, BREAK } command; // ��������

    // ·������
    string normalize_path(const string& path)
    {
        string result = path;
        for (char& c : result) if (c == '\\') c = '/';
        return result;
    }

    // �ļ����
    bool file_exists(const string& path)
    {
        return GetFileAttributesA(normalize_path(path).c_str()) != INVALID_FILE_ATTRIBUTES;
    }

    // ��ȡ����·��
    string get_file_path(const string& filename)
    {
        return musicDir.empty() ? filename : (musicDir + "/" + filename);
    }

    // ������⣨��ȥ������
    bool is_key_pressed(int keyCode)
    {
        static bool keyState[256] = { false };
        bool nowPressed = (GetAsyncKeyState(keyCode) & 0x8000) != 0;

        if (nowPressed && !keyState[keyCode]) {
            keyState[keyCode] = true;
            return true;
        }
        else if (!nowPressed) {
            keyState[keyCode] = false;
        }
        return false;
    }

    // ����ָ�������ĸ���
    bool play_index(int index) {
        if (index < 0 || index >= playlist.size()) {
            cout << "[" << playerName << "] ��Ч�ĸ�������: " << index << endl;
            return false;
        }

        // ֹͣ��ǰ����
        if (isPlaying) {
            mciSendString(L"stop mp3", NULL, 0, NULL);
            mciSendString(L"close mp3", NULL, 0, NULL);
        }

        string filename = playlist[index];
        string file_path = get_file_path(filename);
        wstring w_file_path(file_path.begin(), file_path.end());

        wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
        wstring play_cmd = isSingleLoop ? L"play mp3 repeat" : (isLooping ? L"play mp3 repeat" : L"play mp3");

        // ִ��MCI���������
        MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
        if (error != 0) {
            wchar_t errorMsg[128];
            mciGetErrorString(error, errorMsg, 128);
            wcout << L"[" << playerName.c_str() << L"] ����: " << errorMsg << endl;
            cout << "[" << playerName << "] �޷����ļ�: " << file_path << endl;
            return false;
        }

        error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
        if (error != 0) {
            wchar_t errorMsg[128];
            mciGetErrorString(error, errorMsg, 128);
            wcout << L"[" << playerName.c_str() << L"] ����: " << errorMsg << endl;
            cout << "[" << playerName << "] �޷������ļ�: " << file_path << endl;
            mciSendString(L"close mp3", NULL, 0, NULL);
            return false;
        }

        currentIndex = index;
        isPlaying = true;
        isPaused = false;
        cout << "[" << playerName << "] ���ڲ���: " << playlist[index] << endl;
        return true;
    }

    // �����������
    void process_command() {
        Command cmd;
        {
            lock_guard<mutex> lock(controlMutex);
            cmd = command;
            command = NONE; // ��������
        }

        switch (cmd) {
        case PLAY:
            if (!playlist.empty()) {
                play_index(currentIndex >= 0 ? currentIndex : 0);
            }
            break;
        case PAUSE:
            if (isPlaying && !isPaused) {
                mciSendString(L"pause mp3", NULL, 0, NULL);
                isPaused = true;
                cout << "[" << playerName << "] ����ͣ����" << endl;
            }
            break;
        case RESUME:
            if (isPlaying && isPaused) {
                mciSendString(L"resume mp3", NULL, 0, NULL);
                isPaused = false;
                cout << "[" << playerName << "] �ѻָ�����" << endl;
            }
            break;
        case STOP:
            if (isPlaying) {
                mciSendString(L"stop mp3", NULL, 0, NULL);
                mciSendString(L"close mp3", NULL, 0, NULL);
                isPlaying = false;
                isPaused = false;
                cout << "[" << playerName << "] ������ֹͣ" << endl;
            }
            break;
        case REPLAY:
            if (!playlist.empty()) {
                play_index(0);
                cout << "[" << playerName << "] ��ͷ��ʼ����" << endl;
            }
            break;
        case BREAK:
            if (isPlaying) {
                mciSendString(L"stop mp3", NULL, 0, NULL);
                mciSendString(L"close mp3", NULL, 0, NULL);
            }
            playlist.clear();
            currentIndex = -1;
            isPlaying = false;
            isPaused = false;
            cout << "[" << playerName << "] ������������" << endl;
            break;
        case NONE:
            break;
        }
    }

    // ��Ƶ�����̺߳���
    void audio_play_thread() {
        threadRunning = true;
        command = NONE;

        if (!playlist.empty()) {
            play_index(0);
        }
        else {
            cout << "[" << playerName << "] �赥Ϊ�գ��޷�����" << endl;
            threadRunning = false;
            return;
        }

        cout << "===== " << playerName << " ���ֲ����� =====" << endl;
        cout << "����˵����" << endl;
        cout << "P������ͣ����" << endl;
        cout << "R�����ָ�����" << endl;
        cout << "A������ͷ����" << endl;
        cout << "=====================" << endl;

        // ѭ������������Ͳ���״̬
        while (threadRunning) {
            // �����������
            process_command();

            // ����������
            if (is_key_pressed('P')) {
                pause();
            }
            else if (is_key_pressed('R')) {
                resume();
            }
            else if (is_key_pressed('A')) {
                replay();
            }

            // ����Ƿ񲥷����
            if (isPlaying && !isPaused) {
                wchar_t status[128];
                mciSendString(L"status mp3 mode", status, 128, NULL);

                if (wcscmp(status, L"stopped") == 0 || wcscmp(status, L"completed") == 0) {
                    if (isSingleLoop) {
                        // ����ѭ��
                        play_index(currentIndex);
                    }
                    else if (isLooping && playlist.size() > 1) {
                        // ������һ��
                        currentIndex = (currentIndex + 1) % playlist.size();
                        play_index(currentIndex);
                    }
                    else {
                        // ֹͣ����
                        isPlaying = false;
                        threadRunning = false;
                    }
                }
            }

            // �������ߣ�����CPUռ��
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        // ������Դ
        if (isPlaying) {
            mciSendString(L"stop mp3", NULL, 0, NULL);
            mciSendString(L"close mp3", NULL, 0, NULL);
        }
        cout << "[" << playerName << "] �������߳����˳�" << endl;
    }

public:
    string musicDir = "";       // ����Ŀ¼
    // ���캯��
    explicit AudioPlayer(const string& name) : playerName(name) {}

    // ��ֹ��������͸�ֵ
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    // �ƶ�����͸�ֵ
    AudioPlayer(AudioPlayer&& other) noexcept
        : playerName(move(other.playerName)),
        playlist(move(other.playlist)),
        musicDir(move(other.musicDir)),
        currentIndex(other.currentIndex),
        isPlaying(other.isPlaying),
        isPaused(other.isPaused),
        isLooping(other.isLooping),
        isSingleLoop(other.isSingleLoop),
        threadRunning(other.threadRunning.load()),
        audioThread(move(other.audioThread)),
        command(other.command) {
        other.threadRunning = false;
    }

    AudioPlayer& operator=(AudioPlayer&& other) noexcept {
        if (this != &other) {
            playerName = move(other.playerName);
            playlist = move(other.playlist);
            musicDir = move(other.musicDir);
            currentIndex = other.currentIndex;
            isPlaying = other.isPlaying;
            isPaused = other.isPaused;
            isLooping = other.isLooping;
            isSingleLoop = other.isSingleLoop;
            threadRunning = other.threadRunning.load();
            command = other.command;

            if (audioThread.joinable()) audioThread.detach();
            audioThread = move(other.audioThread);
            other.threadRunning = false;
        }
        return *this;
    }

    // ���ظ赥
    bool load_playlist(const string& playlist_file) {
        lock_guard<mutex> lock(controlMutex);
        playlist.clear();
        musicDir = "";

        // �����赥�ļ�·������ȡĿ¼����
        size_t lastSlash = playlist_file.find_last_of("\\/");
        if (lastSlash != string::npos) {
            musicDir = playlist_file.substr(0, lastSlash);
        }
        else {
            musicDir = ".";
        }

        ifstream file(playlist_file);
        if (!file.is_open()) {
            cout << "[" << playerName << "] �޷��򿪸赥�ļ�: " << playlist_file << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                // ȥ������β�ո�
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);

                string filename = line;
                string fullPath = get_file_path(filename);

                if (file_exists(fullPath)) {
                    playlist.push_back(filename);
                    cout << "[" << playerName << "] �Ѽ��ظ���: " << filename << endl;
                }
                else {
                    cout << "[" << playerName << "] ���������ڣ�����: " << fullPath << endl;
                }
            }
        }

        file.close();
        cout << "[" << playerName << "] �ɹ����ظ赥����" << playlist.size() << "�׸���" << endl;
        return playlist.size() > 0;
    }

    // ���õ���ѭ��
    bool set_single_music(const string& music_file) {
        lock_guard<mutex> lock(controlMutex);

        playlist.clear();
        musicDir = "";

        if (file_exists(music_file)) {
            playlist.push_back(music_file);
            cout << "[" << playerName << "] �����õ���ѭ��: " << music_file << endl;
            return true;
        }
        else {
            cout << "[" << playerName << "] ����������: " << music_file << endl;
            return false;
        }
    }

    // ��ʼ����
    void play() {
        {
            lock_guard<mutex> lock(controlMutex);
            if (threadRunning) {
                stop();
                if (audioThread.joinable()) {
                    audioThread.join();
                }
            }
        }

        audioThread = thread(&AudioPlayer::audio_play_thread, this);
        audioThread.detach();
    }

    // ���Ʒ���
    void stop() {
        {
            lock_guard<mutex> lock(controlMutex);
            command = STOP;
        }
        cmdCV.notify_one();
    }

    void replay() {
        {
            lock_guard<mutex> lock(controlMutex);
            command = REPLAY;
        }
        cmdCV.notify_one();
    }

    void break_the_audio() {
        {
            lock_guard<mutex> lock(controlMutex);
            command = BREAK;
        }
        cmdCV.notify_one();
    }

    void pause() {
        {
            lock_guard<mutex> lock(controlMutex);
            command = PAUSE;
        }
        cmdCV.notify_one();
    }

    void resume() {
        {
            lock_guard<mutex> lock(controlMutex);
            command = RESUME;
        }
        cmdCV.notify_one();
    }

    // ����ѭ��ģʽ
    void set_loop(bool loop) {
        lock_guard<mutex> lock(controlMutex);
        isLooping = loop;
        cout << "[" << playerName << "] ѭ��ģʽ��" << (loop ? "����" : "�ر�") << endl;
    }

    // ���õ���ѭ��ģʽ
    void set_single_loop(bool singleLoop) {
        lock_guard<mutex> lock(controlMutex);
        isSingleLoop = singleLoop;
        cout << "[" << playerName << "] ����ѭ��ģʽ��" << (singleLoop ? "����" : "�ر�") << endl;
    }

    // ��������
    ~AudioPlayer() {
        threadRunning = false;
        cmdCV.notify_one();

        if (audioThread.joinable()) {
            audioThread.join();
        }
    }
};

// ������������
class AudioPlayerFactory {
private:
    static unordered_map<string, shared_ptr<AudioPlayer>> players;

public:
    // ����������
    static shared_ptr<AudioPlayer> createPlayer(const string& name) {
        if (players.find(name) != players.end()) {
            cout << "������ '" << name << "' �Ѵ���" << endl;
            return players[name];
        }

        auto player = make_shared<AudioPlayer>(name);
        players[name] = player;
        return player;
    }

    // ��ȡ������
    static shared_ptr<AudioPlayer> getPlayer(const string& name) {
        auto it = players.find(name);
        if (it != players.end()) {
            return it->second;
        }
        cout << "������ '" << name << "' ������" << endl;
        return nullptr;
    }

    // ɾ��������
    static void deletePlayer(const string& name) {
        auto it = players.find(name);
        if (it != players.end()) {
            it->second->stop();
            players.erase(it);
            cout << "������ '" << name << "' ��ɾ��" << endl;
        }
        else {
            cout << "������ '" << name << "' ������" << endl;
        }
    }
};

unordered_map<string, shared_ptr<AudioPlayer>> AudioPlayerFactory::players;

