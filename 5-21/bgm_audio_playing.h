#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <fstream>
#include <vector>
#include <string>
#pragma comment(lib, "winmm.lib")
using namespace std;

// 全局变量
vector<string> playlist;      // 歌单（仅包含文件名）
string musicDir = "./bgm-playing/";         // 音乐文件所在目录
int currentIndex = -1;        // 当前播放的歌曲索引
bool isPlaying = false;       // 播放状态
bool isPaused = false;        // 暂停状态
bool isLooping = true;        // 循环播放标志

// 规范化路径分隔符（兼容Windows和Unix风格）
string normalize_path(const string& path) {
    string result = path;
    // 将所有反斜杠替换为正斜杠（Windows API兼容两种分隔符）
    for (char& c : result) {
        if (c == '\\') c = '/';
    }
    return result;
}

// 更新后的文件检查函数
bool file_exists(const string& path) {
    string normalized = normalize_path(path);
    DWORD fileAttributes = GetFileAttributesA(normalized.c_str());
    return (fileAttributes != INVALID_FILE_ATTRIBUTES);
}


// 从文件读取歌单
bool load_playlist(const string& playlist_file) {
    playlist.clear();

    // 获取歌单文件所在目录
    size_t lastSlash = playlist_file.find_last_of("\\/");
    if (lastSlash != string::npos) {
        musicDir = playlist_file.substr(0, lastSlash);
    }
    else {
        // 如果没有路径信息，使用当前目录
        musicDir = ".";
    }

    ifstream file(playlist_file);
    if (!file.is_open()) {
        cout << "无法打开歌单文件: " << playlist_file << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            // 检查文件是否存在
            string fullPath = musicDir + "\\" + line;
            if (file_exists(fullPath)) {
                playlist.push_back(line);  // 只保存文件名
                cout << "已加载歌曲: " << line << endl;
            }
            else {
                cout << "歌曲不存在，跳过: " << line << endl;
            }
        }
    }

    file.close();
    cout << "成功加载歌单，共" << playlist.size() << "首歌曲" << endl;
    currentIndex = -1;  // 重置当前索引
    return playlist.size() > 0;
}

// 获取当前歌曲的完整路径
string get_full_path(int index) {
    if (index < 0 || index >= playlist.size()) {
        return "";
    }
    return musicDir + "\\" + playlist[index];
}

bool key_handler()
{
    if (GetAsyncKeyState('P') & 0x8000) return true;
    else if (GetAsyncKeyState('R') & 0x8000) return true;
}

// 播放指定索引的歌曲
bool play_index(int index)
{
    if (index < 0 || index >= playlist.size()) {
        cout << "无效的歌曲索引: " << index << endl;
        return false;
    }

    // 停止当前播放
    if (isPlaying)
    {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
    }

    string file_path = get_full_path(index);
    wstring w_file_path(file_path.begin(), file_path.end());

    wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
    wstring play_cmd = L"play mp3";

    // 执行MCI命令并检查错误
    MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"错误: " << errorMsg << endl;
        cout << "无法打开文件: " << file_path << endl;
        return false;
    }

    error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"错误: " << errorMsg << endl;
        cout << "无法播放文件: " << file_path << endl;
        mciSendString(L"close mp3", NULL, 0, NULL);
        return false;
    }

    currentIndex = index;
    isPlaying = true;
    isPaused = false;
    return true;
}
// 停止播放
void stop(bool t = true) 
{
    if (isPlaying&&!t)
    {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
        isPlaying = false;
        isPaused = false;
    }
}
// 开始自动播放歌单
bool play(const string& playlist_file = "./bgm-playing/bgm.txt")
{
    // 如果正在播放，先停止
    if (isPlaying) {
        stop();
    }

    // 如果歌单为空，尝试重新加载
    if (playlist.empty()) {
        if (!load_playlist(playlist_file)) {
            cout << "歌单为空且加载失败，无法播放" << endl;
            return false;
        }
    }

    // 从第一首开始播放
    return play_index(0);
}



// 重新播放（从头开始）
bool replay() {
    stop();  // 先停止当前播放

    // 如果歌单为空，尝试重新加载
    if (playlist.empty()) {
        if (!load_playlist("./bgm-playing/bgm.txt")) {
            cout << "歌单为空且加载失败，无法播放" << endl;
            return false;
        }
    }

    // 从第一首开始播放
    return play_index(0);
}

// 播放下一首
bool play_next() {
    if (playlist.empty()) return false;

    int nextIndex = (currentIndex + 1) % playlist.size();
    return play_index(nextIndex);
}

// 播放上一首
bool play_previous() {
    if (playlist.empty()) return false;

    int prevIndex = (currentIndex - 1 + playlist.size()) % playlist.size();
    return play_index(prevIndex);
}

// 设置循环模式
void set_loop_mode(bool loop) {
    isLooping = loop;
    cout << "循环模式已" << (loop ? "开启" : "关闭") << endl;
}

// 检查音乐是否播放完毕（需要在循环中调用）
bool check_and_play_next() {
    if (!isPlaying || isPaused) return false;

    // 查询当前播放状态
    wchar_t status[128];
    mciSendString(L"status mp3 mode", status, 128, NULL);

    // 如果播放完毕，根据循环模式决定是否播放下一首
    if (wcscmp(status, L"stopped") == 0 || wcscmp(status, L"completed") == 0) {
        if (isLooping) {
            return play_next();
        }
        else {
            stop();
            return false;
        }
    }

    return false;
}

// 获取当前播放状态
bool is_playing(bool t = true) 
{
    return isPlaying && !isPaused;
}

// 获取当前播放的歌曲名称
string get_current_song_name() 
{
    if (currentIndex < 0 || currentIndex >= playlist.size())
    {
        return "";
    }
    return playlist[currentIndex];
}

// 暂停播放
void pause(bool t = true) 
{
    t = key_handler();
    if (isPlaying && !isPaused && t) {
        mciSendString(L"pause mp3", NULL, 0, NULL);
        isPaused = true;
    }
}

// 恢复播放
void resume(bool t = true) 
{
    t = key_handler();
    if (isPlaying && isPaused && t) {
        mciSendString(L"resume mp3", NULL, 0, NULL);
        isPaused = false;
    }
}


// 单独播放指定文件名的音频（循环播放）
void play_index_only(const string& file_name, bool loop = true) {
    // 检查文件是否存在
    musicDir = "./bgm-ui/";
    string full_path = normalize_path(musicDir + "/" + file_name);
    if (!file_exists(full_path)) {
        cout << "文件不存在: " << full_path << endl;
        return;
    }

    // 停止当前播放
    if (isPlaying) {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
    }

    wstring w_file_path(full_path.begin(), full_path.end());
    wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
    wstring play_cmd = loop ? L"play mp3 repeat" : L"play mp3";

    // 执行播放命令
    MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"错误: " << errorMsg << endl;
        return;
    }

    error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"错误: " << errorMsg << endl;
        mciSendString(L"close mp3", NULL, 0, NULL);
        return;
    }

    // 更新状态（可选：如需记录当前播放文件，可添加逻辑）
    isPlaying = true;
    isPaused = false;
    cout << "正在循环播放: " << file_name << endl;
}


/*
下面是接口声明：
当我们进入到相关的页面的时候，只需要输一下下面的函数的函数名即可,
当然了，页面和相应的接口调用也进行相应的改进的。这个就是前端干的活！
*/
//这个是一些需要循环播放的页面运行的函数
void loopplay()
{
    if (!play("./bgm-playing/bgm.txt"))
    {
        cout << "初始化播放失败，程序退出" << endl;
    }

    cout << "===== 音乐播放器 =====" << endl;
    cout << "操作说明：" << endl;
    cout << "P键：暂停/停止播放" << endl;
    cout << "R键：恢复播放/从头开始" << endl;
    cout << "ESC键：退出程序" << endl;

    bool running = true;
    while (running) {
        // 检测ESC键（退出程序）
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            cout << "检测到ESC键，退出程序..." << endl;
            running = false;
            continue;
        }

        // 检测P键（暂停/停止）
        if (GetAsyncKeyState('P') & 0x8000) {
            if (isPlaying) {
                if (isPaused) {
                    stop();
                    cout << "已停止播放" << endl;
                }
                else {
                    pause();
                    cout << "已暂停播放" << endl;
                }
            }
            else {
                cout << "播放已停止，按R键重新开始" << endl;
            }
        }

        // 检测R键（恢复/重启）
        if (GetAsyncKeyState('R') & 0x8000) {
            if (isPlaying && isPaused) {
                resume();
                cout << "已恢复播放" << endl;
            }
            else {
                replay();
                cout << "从头开始播放" << endl;
            }
        }

        // 自动切歌逻辑
        if (isPlaying && !isPaused) {
            check_and_play_next();
        }

        // 避免CPU占用过高
        Sleep(100);
    }

    // 清理资源
    stop();
    cout << "程序已退出" << endl;
}

//这个是排名页面需要运行的函数接口
void loopplayonly()
{
    cout << "===== 单曲循环播放器 =====" << endl;
    cout << "操作说明：" << endl;
    cout << "P键暂停，R键恢复，ESC键退出" << endl;
    cout << "=====================" << endl;

    // 示例：循环播放名为 "bgm1.mp3" 的文件（需存在于musicDir目录）
    play_index_only("qualifying_bgm.mp3");

    bool running = true;
    while (running) {
        // 检测ESC键退出
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;
            cout << "退出程序..." << endl;
            break;
        }

        // 检测P键暂停
        if (GetAsyncKeyState('P') & 0x8000) {
            if (isPlaying && !isPaused) {
                mciSendString(L"pause mp3", NULL, 0, NULL);
                isPaused = true;
                cout << "已暂停播放" << endl;
            }
        }

        // 检测R键恢复
        if (GetAsyncKeyState('R') & 0x8000) {
            if (isPlaying && isPaused) {
                mciSendString(L"resume mp3", NULL, 0, NULL);
                isPaused = false;
                cout << "已恢复播放" << endl;
            }
        }

        Sleep(100);
    }

    // 停止播放并清理资源
    mciSendString(L"stop mp3", NULL, 0, NULL);
    mciSendString(L"close mp3", NULL, 0, NULL);
    isPlaying = false;
    isPaused = false;
}