#include<iostream>
#include"solution_of_Suduko.h";
#include"page_of_solve.h";
#include"players_num.h"
#include"create_players.h"
#include"scores_of_all_players.h"
#include"bgm_audio_playing.h"
#include"sign_up_and_log_in.h"



using namespace std;
int main() {
    PlayerService service;

    // ����ע���������
    service.quickRegister();
    service.quickRegister();

    // ģ���޸�ĳ�������Ϣ�����ȵ�¼��
    if (service.login("player1", "123456"))
    {
        service.modifyPlayerInfo("new_player1", "654321");
        service.logout();
    }

    // �������������Ϣ���ļ��У�����ԭ���ļ���
    service.save_to_folder();

    return 0;
}

/*
�����˺Ź��ܵ���Ʋ��֣�

#include"players_num.h"
#include"create_players.h"
    // ����1��ʹ��create_new_player�����Զ�����
    create_new_player("./players/");  // ����player1

    // ����2���ֶ�����������
    Player custom("��", "54321", 100, 50, 20);
    custom.saveToFile("./players/");

    // ����3�������������
    Player loaded;
    loaded.loadFromFile("./players/��.txt");
    std::cout << "���ص����: " << loaded.getName() << std::endl;

    // �޸Ĳ�����
    loaded.reset("warrior_master");
    loaded.setScores(150, 80, 40);
    loaded.saveToFile("./players/");
    return 0;
*/

/*
���ڻ�ȡ������ҵķ�������Ʋ��֣�����ʾ�Ĺ���
    string folder = "./players/";

    // ��ȡ�������ͳ������
    auto stats = readAllPlayerStats(folder);

    // ��ӡ�����������
    printAllPlayerStats(stats);

    // �����ܷ�
    int totalSimple = getTotalScoreByMode(stats, GameMode::Simple);
    int totalMedium = getTotalScoreByMode(stats, GameMode::Medium);
    int totalHard = getTotalScoreByMode(stats, GameMode::Hard);

    cout << "\n��������ܷ�:" << endl;
    cout << "��ģʽ�ܷ�: " << totalSimple << endl;
    cout << "�е�ģʽ�ܷ�: " << totalMedium << endl;
    cout << "����ģʽ�ܷ�: " << totalHard << endl;

    // ������߷����
    auto topSimple = getTopPlayerByMode(stats, GameMode::Simple);
    auto topMedium = getTopPlayerByMode(stats, GameMode::Medium);
    auto topHard = getTopPlayerByMode(stats, GameMode::Hard);

    cout << "\n��ģʽ��߷����:" << endl;
    cout << "��ģʽ: " << topSimple.name << " (" << topSimple.simpleMode << ")" << endl;
    cout << "�е�ģʽ: " << topMedium.name << " (" << topMedium.mediumMode << ")" << endl;
    cout << "����ģʽ: " << topHard.name << " (" << topHard.hardMode << ")" << endl;
*/

/*
�������ֵĳ�����ʾ������

����ǹ������������Ҫ������Ȱ�ť���Զ�����ͣ�Ͳ��ŵĹ��ܵļ�ʵ�֣�����ʵ�ֵĻ���Ҫ��gui��
 if (!play("./bgm-ui/bgm.txt")) {
        cout << "��ʼ������ʧ�ܣ������˳�" << endl;
        return 1;
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
    return 0;

    �����ǵ���ѭ���Ĳ��ŷ���:
int main()
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
    return 0;
}
*/

/*������ʾ����һ����ҵ��˺Ŵ�������Ϣ���޸ģ����еĶ˿���setBestScore��incrementLevelCount
* Ҳ����˵һ����ɺ���һ�ֵķ����ᱻ��¼����
    try
    {
        // 1. ������������Ҷ���
        std::cout << "\n===== ��������� =====" << std::endl;

        // �������1��ʹ��Ĭ�Ϲ��캯�����Զ��������ƣ�
        Player player1;
        player1.setBestScore(120.5, "simple");  // ���ü�ģʽ�ɼ�
        player1.incrementLevelCount("simple");   // ��ģʽͨ��+1
        player1.saveToFile();                    // ���浽Ĭ��·�� ./players/
        std::cout << "�������: " << player1.getName() << std::endl;

        // �������2��ָ�����ƺ�����
        Player player2("Bob", "secure123");
        player2.setBestScore(85.0, "simple");    // ��ģʽ��ѳɼ�
        player2.setBestScore(150.0, "mid");   // �е�ģʽ��ѳɼ�
        player2.incrementLevelCount("simple");    // ��ģʽͨ��+1
        player2.incrementLevelCount("mid");    // �е�ģʽͨ��+1
        player2.saveToFile();
        std::cout << "�������: " << player2.getName() << std::endl;

        // �������3��ʹ�þ�̬�������Զ��������ƣ�
        Player::createNewPlayer("./players/");    // ���þ�̬��������
        std::cout << "��ǰ�������: " << Player::getPlayerCount() << std::endl;

        // 2. ���ļ�����������ݣ������2Ϊ����
        std::cout << "\n===== ���������Ϣ =====" << std::endl;
        Player loadedPlayer;
        std::string filePath = "./players/Bob.txt";

        if (loadedPlayer.loadFromFile(filePath)) {
            std::cout << "�������: " << loadedPlayer.getName() << std::endl;
            std::cout << "��ģʽ�ɼ�: " << loadedPlayer.getBestSimpleScore() << " ��" << std::endl;
            std::cout << "�е�ģʽͨ�ش���: " << loadedPlayer.getMidModeCount() << std::endl;
        }
        else {
            std::cerr << "����ʧ��: " << filePath << std::endl;
        }

        // 3. �޸������Ϣ�����±���
        std::cout << "\n===== �޸������Ϣ =====" << std::endl;
        loadedPlayer.reset("Bob_Updated", "newPassword456");  // �޸����ƺ�����
        loadedPlayer.setBestScore(200.0, "hard");             // ��������ģʽ�ɼ�
        loadedPlayer.incrementLevelCount("hard");              // ����ģʽͨ��+1
        loadedPlayer.saveToFile();                             // �����޸ĺ������

        std::cout << "�޸ĺ������: " << loadedPlayer.getName() << std::endl;
        std::cout << "����ģʽ��ѳɼ�: " << loadedPlayer.getBestHardScore() << " ��" << std::endl;

        // 4. ��ȡ���������Ϣ����ӡ
        std::cout << "\n===== ���������Ϣ =====" << std::endl;
        auto allPlayers = get_all_info();

        for (const auto& info : allPlayers) {
            std::cout << "\n���: " << info.name << std::endl;
            std::cout << "��ģʽ: " << (info.simpleTime < 999999.9 ?
                std::to_string(info.simpleTime) + " ��" : "δ���") << std::endl;
            std::cout << "�е�ģʽ: " << (info.mediumTime < 999999.9 ?
                std::to_string(info.mediumTime) + " ��" : "δ���") << std::endl;
            std::cout << "����ģʽ: " << (info.hardTime < 999999.9 ?
                std::to_string(info.hardTime) + " ��" : "δ���") << std::endl;
        }

        // 5. ������ģʽ���򲢴�ӡ����  ���ֻ�����ӣ���ģʽ���е�ģʽ��������һ������
        std::cout << "\n===== ����ģʽ���� =====" << std::endl;
        sort_the_players(allPlayers, GameMode::Hard);

        for (size_t i = 0; i < allPlayers.size(); ++i) {
            std::cout << "�� " << i + 1 << " ��: " << allPlayers[i].name
                << " - " << allPlayers[i].hardTime << " ��" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\n����: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/