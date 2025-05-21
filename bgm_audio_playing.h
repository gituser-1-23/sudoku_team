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

// 播放器配置类
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
    string playerName;          // 播放器名称
    vector<string> playlist;    // 歌单

    int currentIndex = -1;      // 当前播放索引
    bool isPlaying = false;     // 播放状态
    bool isPaused = false;      // 暂停状态
    bool isLooping = true;      // 循环模式
    bool isSingleLoop = false;  // 单曲循环标志
    atomic<bool> threadRunning{ false }; // 线程运行标志
    thread audioThread;         // 音频线程
    mutex controlMutex;         // 控制锁
    condition_variable cmdCV;   // 命令条件变量
    enum Command { NONE, PLAY, PAUSE, RESUME, STOP, REPLAY, BREAK } command; // 控制命令

    // 路径处理
    string normalize_path(const string& path)
    {
        string result = path;
        for (char& c : result) if (c == '\\') c = '/';
        return result;
    }

    // 文件检查
    bool file_exists(const string& path)
    {
        return GetFileAttributesA(normalize_path(path).c_str()) != INVALID_FILE_ATTRIBUTES;
    }

    // 获取完整路径
    string get_file_path(const string& filename)
    {
        return musicDir.empty() ? filename : (musicDir + "/" + filename);
    }

    // 按键检测（带去抖动）
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

    // 播放指定索引的歌曲
    bool play_index(int index) {
        if (index < 0 || index >= playlist.size()) {
            cout << "[" << playerName << "] 无效的歌曲索引: " << index << endl;
            return false;
        }

        // 停止当前播放
        if (isPlaying) {
            mciSendString(L"stop mp3", NULL, 0, NULL);
            mciSendString(L"close mp3", NULL, 0, NULL);
        }

        string filename = playlist[index];
        string file_path = get_file_path(filename);
        wstring w_file_path(file_path.begin(), file_path.end());

        wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
        wstring play_cmd = isSingleLoop ? L"play mp3 repeat" : (isLooping ? L"play mp3 repeat" : L"play mp3");

        // 执行MCI命令并检查错误
        MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
        if (error != 0) {
            wchar_t errorMsg[128];
            mciGetErrorString(error, errorMsg, 128);
            wcout << L"[" << playerName.c_str() << L"] 错误: " << errorMsg << endl;
            cout << "[" << playerName << "] 无法打开文件: " << file_path << endl;
            return false;
        }

        error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
        if (error != 0) {
            wchar_t errorMsg[128];
            mciGetErrorString(error, errorMsg, 128);
            wcout << L"[" << playerName.c_str() << L"] 错误: " << errorMsg << endl;
            cout << "[" << playerName << "] 无法播放文件: " << file_path << endl;
            mciSendString(L"close mp3", NULL, 0, NULL);
            return false;
        }

        currentIndex = index;
        isPlaying = true;
        isPaused = false;
        cout << "[" << playerName << "] 正在播放: " << playlist[index] << endl;
        return true;
    }

    // 处理控制命令
    void process_command() {
        Command cmd;
        {
            lock_guard<mutex> lock(controlMutex);
            cmd = command;
            command = NONE; // 重置命令
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
                cout << "[" << playerName << "] 已暂停播放" << endl;
            }
            break;
        case RESUME:
            if (isPlaying && isPaused) {
                mciSendString(L"resume mp3", NULL, 0, NULL);
                isPaused = false;
                cout << "[" << playerName << "] 已恢复播放" << endl;
            }
            break;
        case STOP:
            if (isPlaying) {
                mciSendString(L"stop mp3", NULL, 0, NULL);
                mciSendString(L"close mp3", NULL, 0, NULL);
                isPlaying = false;
                isPaused = false;
                cout << "[" << playerName << "] 播放已停止" << endl;
            }
            break;
        case REPLAY:
            if (!playlist.empty()) {
                play_index(0);
                cout << "[" << playerName << "] 从头开始播放" << endl;
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
            cout << "[" << playerName << "] 播放器已重置" << endl;
            break;
        case NONE:
            break;
        }
    }

    // 音频播放线程函数
    void audio_play_thread() {
        threadRunning = true;
        command = NONE;

        if (!playlist.empty()) {
            play_index(0);
        }
        else {
            cout << "[" << playerName << "] 歌单为空，无法播放" << endl;
            threadRunning = false;
            return;
        }

        cout << "===== " << playerName << " 音乐播放器 =====" << endl;
        cout << "操作说明：" << endl;
        cout << "P键：暂停播放" << endl;
        cout << "R键：恢复播放" << endl;
        cout << "A键：从头播放" << endl;
        cout << "=====================" << endl;

        // 循环检测键盘输入和播放状态
        while (threadRunning) {
            // 处理控制命令
            process_command();

            // 检测键盘输入
            if (is_key_pressed('P')) {
                pause();
            }
            else if (is_key_pressed('R')) {
                resume();
            }
            else if (is_key_pressed('A')) {
                replay();
            }

            // 检查是否播放完毕
            if (isPlaying && !isPaused) {
                wchar_t status[128];
                mciSendString(L"status mp3 mode", status, 128, NULL);

                if (wcscmp(status, L"stopped") == 0 || wcscmp(status, L"completed") == 0) {
                    if (isSingleLoop) {
                        // 单曲循环
                        play_index(currentIndex);
                    }
                    else if (isLooping && playlist.size() > 1) {
                        // 播放下一首
                        currentIndex = (currentIndex + 1) % playlist.size();
                        play_index(currentIndex);
                    }
                    else {
                        // 停止播放
                        isPlaying = false;
                        threadRunning = false;
                    }
                }
            }

            // 短暂休眠，减少CPU占用
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        // 清理资源
        if (isPlaying) {
            mciSendString(L"stop mp3", NULL, 0, NULL);
            mciSendString(L"close mp3", NULL, 0, NULL);
        }
        cout << "[" << playerName << "] 播放器线程已退出" << endl;
    }

public:
    string musicDir = "";       // 音乐目录
    // 构造函数
    explicit AudioPlayer(const string& name) : playerName(name) {}

    // 禁止拷贝构造和赋值
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    // 移动构造和赋值
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

    // 加载歌单
    bool load_playlist(const string& playlist_file) {
        lock_guard<mutex> lock(controlMutex);
        playlist.clear();
        musicDir = "";

        // 解析歌单文件路径，获取目录部分
        size_t lastSlash = playlist_file.find_last_of("\\/");
        if (lastSlash != string::npos) {
            musicDir = playlist_file.substr(0, lastSlash);
        }
        else {
            musicDir = ".";
        }

        ifstream file(playlist_file);
        if (!file.is_open()) {
            cout << "[" << playerName << "] 无法打开歌单文件: " << playlist_file << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                // 去除行首尾空格
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);

                string filename = line;
                string fullPath = get_file_path(filename);

                if (file_exists(fullPath)) {
                    playlist.push_back(filename);
                    cout << "[" << playerName << "] 已加载歌曲: " << filename << endl;
                }
                else {
                    cout << "[" << playerName << "] 歌曲不存在，跳过: " << fullPath << endl;
                }
            }
        }

        file.close();
        cout << "[" << playerName << "] 成功加载歌单，共" << playlist.size() << "首歌曲" << endl;
        return playlist.size() > 0;
    }

    // 设置单曲循环
    bool set_single_music(const string& music_file) {
        lock_guard<mutex> lock(controlMutex);

        playlist.clear();
        musicDir = "";

        if (file_exists(music_file)) {
            playlist.push_back(music_file);
            cout << "[" << playerName << "] 已设置单曲循环: " << music_file << endl;
            return true;
        }
        else {
            cout << "[" << playerName << "] 歌曲不存在: " << music_file << endl;
            return false;
        }
    }

    // 开始播放
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

    // 控制方法
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

    // 设置循环模式
    void set_loop(bool loop) {
        lock_guard<mutex> lock(controlMutex);
        isLooping = loop;
        cout << "[" << playerName << "] 循环模式已" << (loop ? "开启" : "关闭") << endl;
    }

    // 设置单曲循环模式
    void set_single_loop(bool singleLoop) {
        lock_guard<mutex> lock(controlMutex);
        isSingleLoop = singleLoop;
        cout << "[" << playerName << "] 单曲循环模式已" << (singleLoop ? "开启" : "关闭") << endl;
    }

    // 析构函数
    ~AudioPlayer() {
        threadRunning = false;
        cmdCV.notify_one();

        if (audioThread.joinable()) {
            audioThread.join();
        }
    }
};

// 播放器工厂类
class AudioPlayerFactory {
private:
    static unordered_map<string, shared_ptr<AudioPlayer>> players;

public:
    // 创建播放器
    static shared_ptr<AudioPlayer> createPlayer(const string& name) {
        if (players.find(name) != players.end()) {
            cout << "播放器 '" << name << "' 已存在" << endl;
            return players[name];
        }

        auto player = make_shared<AudioPlayer>(name);
        players[name] = player;
        return player;
    }

    // 获取播放器
    static shared_ptr<AudioPlayer> getPlayer(const string& name) {
        auto it = players.find(name);
        if (it != players.end()) {
            return it->second;
        }
        cout << "播放器 '" << name << "' 不存在" << endl;
        return nullptr;
    }

    // 删除播放器
    static void deletePlayer(const string& name) {
        auto it = players.find(name);
        if (it != players.end()) {
            it->second->stop();
            players.erase(it);
            cout << "播放器 '" << name << "' 已删除" << endl;
        }
        else {
            cout << "播放器 '" << name << "' 不存在" << endl;
        }
    }
};

unordered_map<string, shared_ptr<AudioPlayer>> AudioPlayerFactory::players;

