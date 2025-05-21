#include<iostream>
#include"solution_of_Suduko.h";
#include"players_num.h"
#include"create_players.h"
#include"scores_of_all_players.h"
#include"player_system.h"
#include"sort_the_players.h"
#include"bgm_audio_playing.h"
#include"prompt_function.h"
using namespace std;



// ��������� automatic_debugging �����Ѿ�����

int main()
{
    PlayerService service;
    // 1. ���ע��Ͳ���ʾ��
    try 
    {
        vector<vector<int>> test1 = 
        {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
        };
        // ��¼/ע���߼�

        service.quickRegister();
        string playername, password;
        cout << "�����������:" << endl;
        cin >> playername;
        cout << "��������:" << endl;
        cin >> password;

        // ��֤��¼
        service.check_and_login(playername, password);
        Player* player1 = service.getCurrentPlayer();
        service.check_and_logout();

        //4.����ÿһ��ÿһ����ҵļ�ʱ
        cout << "�����������:" << endl;
        cin >> playername;
        cout << "��������:" << endl;
        cin >> password;
        service.check_and_login(playername, password);
        player1 = service.getCurrentPlayer();
        player1->showcurrent_suduko();
        player1->setmode("simple");
        for (int i = 1; i < 4; i++)
        {
            player1->update_time(i*2);
        }
        cout << player1->getcurrent_time() << endl;
        service.check_and_logout();
        service.save_to_folder();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n����: " << e.what() << std::endl;
        return 1;
    }
    return 0;

    
}
/*
���ǵ��˺Ź���ϵͳ��ʹ�÷������°�����
�������ǵĹ���ϵͳ�ǻ���ҳ�濪���ģ������һЩ�����Ĵ����������;Ͳ�����const string�ˣ���������ǰ�ĺ������������޸�
player1
123456

player1
123456
����һ��
    try
    {
        // 1. ���ע��Ͳ���ʾ��
        std::cout << "\n===== ���ע�� =====" << std::endl;

        // 2. ʹ�� PlayerService ����в���
        PlayerService service;
        std::cout << "\n===== ʹ�� PlayerService �� =====" << std::endl;
        service.show();

        // ��¼/ע���߼�
        string playername, password;
        cout << "�����������:" << endl;
        cin >> playername;
        cout << "��������:" << endl;
        cin >> password;

        // ��֤��¼
        service.check_and_login(playername,password);
        Player* player1 = service.getCurrentPlayer();
        player1.setmode("hard");
        cout << "������óɼ�(����):" << endl;
        double bst;
        cin >> bst;
        player1->setBestScore(bst);
        service.check_and_logout();
        // 3. ��ȡ���������Ϣ������
        std::cout << "\n===== ��ȡ���������Ϣ������ =====" << std::endl;
        auto allPlayers = get_the_info();

        for (int i = 0; i < 3; i++)
        {
            cout << allPlayers[i] << endl;
        }
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n����: " << e.what() << std::endl;
		return 1;
	}

    ��������
        try
    {
        PlayerService service;
        // 1. ���ע��Ͳ���ʾ��
        std::cout << "\n===== ���ע�� =====" << std::endl;
        for (int i = 0; i < 3; i++)
        {
            string name1, pass1;
            cin >> name1;
            cin >> pass1;
            service.registerPlayer(name1, pass1);
        }
        // 2. ʹ�� PlayerService ����в���

        std::cout << "\n===== ʹ�� PlayerService �� =====" << std::endl;
        service.show();

        // ��¼/ע���߼�
        string playername, password;
        cout << "�����������:" << endl;
        cin >> playername;
        cout << "��������:" << endl;
        cin >> password;

        // ��֤��¼
        service.check_and_login(playername, password);
        Player* player1 = service.getCurrentPlayer();
        cout << "������óɼ�(����):" << endl;
        double bst;
        cin >> bst;
        player1->setBestScore(bst, "hard");
        service.check_and_logout();
        // 3. ��ȡ���������Ϣ������
        std::cout << "\n===== ��ȡ���������Ϣ������ =====" << std::endl;
        auto allPlayers = get_the_info();

        for (int i = 0; i < 3; i++)
        {
            cout << allPlayers[i] << endl;
        }
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n����: " << e.what() << std::endl;
        return 1;
    }

*/

/*
�������ֵĳ�����ʾ������

����ǹ������������Ҫ������Ȱ�ť���Զ�����ͣ�Ͳ��ŵĹ��ܵļ�ʵ�֣�����ʵ�ֵĻ���Ҫ��gui��
ע�����������ֵ�this_thread ��ʾ�������ǵ����̣߳���Ϊ������Ƶ�Ĳ�������������߳���ʵ�ֵģ��������̳߳�ͻ!

  // ����������ͬ��;�Ĳ�����
    auto bgmPlayer = AudioPlayerFactory::createPlayer("��������");
    auto battlePlayer = AudioPlayerFactory::createPlayer("��λ����");

    // ���ض�ӦĿ¼�ĸ赥�ļ�
    // �������ָ赥·����./bgm-playing/bgm.txt
    if (!bgmPlayer->load_playlist("./bgm-playing/bgm.txt")) {
        cout << "�������ָ赥����ʧ�ܣ������˳�" << endl;
        return 1;
    }

    // ��λ���ָ赥·����./bgm-ui/battle.txt
    if (!battlePlayer->load_playlist("./bgm-ui/bgm.txt")) {
        cout << "��λ���ָ赥����ʧ�ܣ������˳�" << endl;
        return 1;
    }

    // ---------------------- �����������߼� ---------------------- //

    // ������������
    bgmPlayer->play();
    cout << "��������������..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // ����5��  

    // ������λ���֣��뱳������ͬʱ���ţ�
    battlePlayer->play();
    cout << "��λ����������..." << endl;
    this_thread::sleep_for(chrono::seconds(3)); // ͬʱ����3��

    // ��ͣ��������
    bgmPlayer->pause();
    cout << "������������ͣ..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // ��ͣ״̬����5��

    // �ָ��������ֲ���
    bgmPlayer->resume();
    cout << "���������ѻָ�..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // �ָ�����5��

    // ֹͣ�������ڲ��ŵ�����
    bgmPlayer->stop();
    battlePlayer->stop();
    cout << "����������ֹͣ..." << endl;

    // ---------------------- ����ѭ��ʾ�� ---------------------- //

    // �����µĵ���ѭ����������ע��������Ψһ���˴�ʹ��"�˵�����"�����ظ���
    auto menuPlayer = AudioPlayerFactory::createPlayer("�˵�����");

    // ���õ���ѭ���ļ��������ļ�·����ȷ��
    if (menuPlayer->set_single_music("./bgm-ui/qualifying_bgm.mp3")) {
        menuPlayer->set_single_loop(true); // ��������ѭ��ģʽ
        menuPlayer->play();
        cout << "����ѭ��������..." << endl;
        this_thread::sleep_for(chrono::seconds(10)); // ����10��
        menuPlayer->stop();
        cout << "����ѭ����ֹͣ..." << endl;
    }
    else {
        cout << "�����ļ�����ʧ��" << endl;
    }

    // ---------------------- ��Դ���� ---------------------- //

    // ɾ�����в���������ѡ�������������Զ�������Դ��
    AudioPlayerFactory::deletePlayer("��������");
    AudioPlayerFactory::deletePlayer("��λ����");
    AudioPlayerFactory::deletePlayer("�˵�����");

    cout << "����ִ�����" << endl;
    return 0;
*/

/*
* ��������ʾ����ĵײ��߼�����ʾ������10����������
* ע�ⷵ�ص�����������string�ģ�Ȼ�����������ֱ�ӳ��������ǵı�����ʾ�������.
    // ��������1����ȷ������
    vector<vector<int>> test1 = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // ��������2����һ�����ظ�����3
    vector<vector<int>> test2 = {
        {5, 3, 4, 6, 7, 3, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // ��������3����һ�����ظ�����5
    vector<vector<int>> test3 = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {5, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // ��������4�����Խ������ظ�����5
    vector<vector<int>> test4 = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 5, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 5, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // ��������5�����Խ������ظ�����2
    vector<vector<int>> test5 = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 2}
    };

    // ��������6����һ�к�Ϊ46������
    vector<vector<int>> test6 = {
        {5, 3, 4, 6, 7, 9, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    // ��������7��������Ч����10
    vector<vector<int>> test7 = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 10}
    };

    // ��������8��������䵫�޴���
    vector<vector<int>> test8 = {
        {5, 3, 4, 6, 7, 8, 0, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 0},
        {1, 9, 8, 3, 0, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 0}
    };

    // ��������9��������������ظ�����
    vector<vector<int>> test9 = {
        {5, 3, 4, 6, 7, 8, 0, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 0},
        {1, 9, 8, 3, 0, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 3}
    };

    // ��������10�����Խ������������ʹ���
    vector<vector<int>> test10 = {
        {6, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 6, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 6, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 6, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 6, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 6, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 6, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 6, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 6}
    };

    // ִ�в���
    vector<vector<vector<int>>> tests = { test1, test2, test3, test4, test5, test6, test7, test8, test9, test10 };

    for (int i = 0; i < tests.size(); i++)
    {
        cout << "�������� " << i + 1 << " �Ľ��:" << endl;
        cout << automatic_debugging(tests[i]) << endl;
        cout << "----------------------------------------" << endl;
    }

    return 0;
*/

