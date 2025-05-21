#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <fstream>
#include <vector>
#include <string>
#pragma comment(lib, "winmm.lib")
using namespace std;

// ȫ�ֱ���
vector<string> playlist;      // �赥���������ļ�����
string musicDir = "./bgm-playing/";         // �����ļ�����Ŀ¼
int currentIndex = -1;        // ��ǰ���ŵĸ�������
bool isPlaying = false;       // ����״̬
bool isPaused = false;        // ��ͣ״̬
bool isLooping = true;        // ѭ�����ű�־

// �淶��·���ָ���������Windows��Unix���
string normalize_path(const string& path) {
    string result = path;
    // �����з�б���滻Ϊ��б�ܣ�Windows API�������ַָ�����
    for (char& c : result) {
        if (c == '\\') c = '/';
    }
    return result;
}

// ���º���ļ���麯��
bool file_exists(const string& path) {
    string normalized = normalize_path(path);
    DWORD fileAttributes = GetFileAttributesA(normalized.c_str());
    return (fileAttributes != INVALID_FILE_ATTRIBUTES);
}


// ���ļ���ȡ�赥
bool load_playlist(const string& playlist_file) {
    playlist.clear();

    // ��ȡ�赥�ļ�����Ŀ¼
    size_t lastSlash = playlist_file.find_last_of("\\/");
    if (lastSlash != string::npos) {
        musicDir = playlist_file.substr(0, lastSlash);
    }
    else {
        // ���û��·����Ϣ��ʹ�õ�ǰĿ¼
        musicDir = ".";
    }

    ifstream file(playlist_file);
    if (!file.is_open()) {
        cout << "�޷��򿪸赥�ļ�: " << playlist_file << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            // ����ļ��Ƿ����
            string fullPath = musicDir + "\\" + line;
            if (file_exists(fullPath)) {
                playlist.push_back(line);  // ֻ�����ļ���
                cout << "�Ѽ��ظ���: " << line << endl;
            }
            else {
                cout << "���������ڣ�����: " << line << endl;
            }
        }
    }

    file.close();
    cout << "�ɹ����ظ赥����" << playlist.size() << "�׸���" << endl;
    currentIndex = -1;  // ���õ�ǰ����
    return playlist.size() > 0;
}

// ��ȡ��ǰ����������·��
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

// ����ָ�������ĸ���
bool play_index(int index)
{
    if (index < 0 || index >= playlist.size()) {
        cout << "��Ч�ĸ�������: " << index << endl;
        return false;
    }

    // ֹͣ��ǰ����
    if (isPlaying)
    {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
    }

    string file_path = get_full_path(index);
    wstring w_file_path(file_path.begin(), file_path.end());

    wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
    wstring play_cmd = L"play mp3";

    // ִ��MCI���������
    MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"����: " << errorMsg << endl;
        cout << "�޷����ļ�: " << file_path << endl;
        return false;
    }

    error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"����: " << errorMsg << endl;
        cout << "�޷������ļ�: " << file_path << endl;
        mciSendString(L"close mp3", NULL, 0, NULL);
        return false;
    }

    currentIndex = index;
    isPlaying = true;
    isPaused = false;
    return true;
}
// ֹͣ����
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
// ��ʼ�Զ����Ÿ赥
bool play(const string& playlist_file = "./bgm-playing/bgm.txt")
{
    // ������ڲ��ţ���ֹͣ
    if (isPlaying) {
        stop();
    }

    // ����赥Ϊ�գ��������¼���
    if (playlist.empty()) {
        if (!load_playlist(playlist_file)) {
            cout << "�赥Ϊ���Ҽ���ʧ�ܣ��޷�����" << endl;
            return false;
        }
    }

    // �ӵ�һ�׿�ʼ����
    return play_index(0);
}



// ���²��ţ���ͷ��ʼ��
bool replay() {
    stop();  // ��ֹͣ��ǰ����

    // ����赥Ϊ�գ��������¼���
    if (playlist.empty()) {
        if (!load_playlist("./bgm-playing/bgm.txt")) {
            cout << "�赥Ϊ���Ҽ���ʧ�ܣ��޷�����" << endl;
            return false;
        }
    }

    // �ӵ�һ�׿�ʼ����
    return play_index(0);
}

// ������һ��
bool play_next() {
    if (playlist.empty()) return false;

    int nextIndex = (currentIndex + 1) % playlist.size();
    return play_index(nextIndex);
}

// ������һ��
bool play_previous() {
    if (playlist.empty()) return false;

    int prevIndex = (currentIndex - 1 + playlist.size()) % playlist.size();
    return play_index(prevIndex);
}

// ����ѭ��ģʽ
void set_loop_mode(bool loop) {
    isLooping = loop;
    cout << "ѭ��ģʽ��" << (loop ? "����" : "�ر�") << endl;
}

// ��������Ƿ񲥷���ϣ���Ҫ��ѭ���е��ã�
bool check_and_play_next() {
    if (!isPlaying || isPaused) return false;

    // ��ѯ��ǰ����״̬
    wchar_t status[128];
    mciSendString(L"status mp3 mode", status, 128, NULL);

    // ���������ϣ�����ѭ��ģʽ�����Ƿ񲥷���һ��
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

// ��ȡ��ǰ����״̬
bool is_playing(bool t = true) 
{
    return isPlaying && !isPaused;
}

// ��ȡ��ǰ���ŵĸ�������
string get_current_song_name() 
{
    if (currentIndex < 0 || currentIndex >= playlist.size())
    {
        return "";
    }
    return playlist[currentIndex];
}

// ��ͣ����
void pause(bool t = true) 
{
    t = key_handler();
    if (isPlaying && !isPaused && t) {
        mciSendString(L"pause mp3", NULL, 0, NULL);
        isPaused = true;
    }
}

// �ָ�����
void resume(bool t = true) 
{
    t = key_handler();
    if (isPlaying && isPaused && t) {
        mciSendString(L"resume mp3", NULL, 0, NULL);
        isPaused = false;
    }
}


// ��������ָ���ļ�������Ƶ��ѭ�����ţ�
void play_index_only(const string& file_name, bool loop = true) {
    // ����ļ��Ƿ����
    musicDir = "./bgm-ui/";
    string full_path = normalize_path(musicDir + "/" + file_name);
    if (!file_exists(full_path)) {
        cout << "�ļ�������: " << full_path << endl;
        return;
    }

    // ֹͣ��ǰ����
    if (isPlaying) {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
    }

    wstring w_file_path(full_path.begin(), full_path.end());
    wstring open_cmd = L"open \"" + w_file_path + L"\" type mpegvideo alias mp3";
    wstring play_cmd = loop ? L"play mp3 repeat" : L"play mp3";

    // ִ�в�������
    MCIERROR error = mciSendString(open_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"����: " << errorMsg << endl;
        return;
    }

    error = mciSendString(play_cmd.c_str(), NULL, 0, NULL);
    if (error != 0) {
        wchar_t errorMsg[128];
        mciGetErrorString(error, errorMsg, 128);
        wcout << L"����: " << errorMsg << endl;
        mciSendString(L"close mp3", NULL, 0, NULL);
        return;
    }

    // ����״̬����ѡ�������¼��ǰ�����ļ���������߼���
    isPlaying = true;
    isPaused = false;
    cout << "����ѭ������: " << file_name << endl;
}


/*
�����ǽӿ�������
�����ǽ��뵽��ص�ҳ���ʱ��ֻ��Ҫ��һ������ĺ����ĺ���������,
��Ȼ�ˣ�ҳ�����Ӧ�Ľӿڵ���Ҳ������Ӧ�ĸĽ��ġ��������ǰ�˸ɵĻ
*/
//�����һЩ��Ҫѭ�����ŵ�ҳ�����еĺ���
void loopplay()
{
    if (!play("./bgm-playing/bgm.txt"))
    {
        cout << "��ʼ������ʧ�ܣ������˳�" << endl;
    }

    cout << "===== ���ֲ����� =====" << endl;
    cout << "����˵����" << endl;
    cout << "P������ͣ/ֹͣ����" << endl;
    cout << "R�����ָ�����/��ͷ��ʼ" << endl;
    cout << "ESC�����˳�����" << endl;

    bool running = true;
    while (running) {
        // ���ESC�����˳�����
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            cout << "��⵽ESC�����˳�����..." << endl;
            running = false;
            continue;
        }

        // ���P������ͣ/ֹͣ��
        if (GetAsyncKeyState('P') & 0x8000) {
            if (isPlaying) {
                if (isPaused) {
                    stop();
                    cout << "��ֹͣ����" << endl;
                }
                else {
                    pause();
                    cout << "����ͣ����" << endl;
                }
            }
            else {
                cout << "������ֹͣ����R�����¿�ʼ" << endl;
            }
        }

        // ���R�����ָ�/������
        if (GetAsyncKeyState('R') & 0x8000) {
            if (isPlaying && isPaused) {
                resume();
                cout << "�ѻָ�����" << endl;
            }
            else {
                replay();
                cout << "��ͷ��ʼ����" << endl;
            }
        }

        // �Զ��и��߼�
        if (isPlaying && !isPaused) {
            check_and_play_next();
        }

        // ����CPUռ�ù���
        Sleep(100);
    }

    // ������Դ
    stop();
    cout << "�������˳�" << endl;
}

//���������ҳ����Ҫ���еĺ����ӿ�
void loopplayonly()
{
    cout << "===== ����ѭ�������� =====" << endl;
    cout << "����˵����" << endl;
    cout << "P����ͣ��R���ָ���ESC���˳�" << endl;
    cout << "=====================" << endl;

    // ʾ����ѭ��������Ϊ "bgm1.mp3" ���ļ����������musicDirĿ¼��
    play_index_only("qualifying_bgm.mp3");

    bool running = true;
    while (running) {
        // ���ESC���˳�
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;
            cout << "�˳�����..." << endl;
            break;
        }

        // ���P����ͣ
        if (GetAsyncKeyState('P') & 0x8000) {
            if (isPlaying && !isPaused) {
                mciSendString(L"pause mp3", NULL, 0, NULL);
                isPaused = true;
                cout << "����ͣ����" << endl;
            }
        }

        // ���R���ָ�
        if (GetAsyncKeyState('R') & 0x8000) {
            if (isPlaying && isPaused) {
                mciSendString(L"resume mp3", NULL, 0, NULL);
                isPaused = false;
                cout << "�ѻָ�����" << endl;
            }
        }

        Sleep(100);
    }

    // ֹͣ���Ų�������Դ
    mciSendString(L"stop mp3", NULL, 0, NULL);
    mciSendString(L"close mp3", NULL, 0, NULL);
    isPlaying = false;
    isPaused = false;
}