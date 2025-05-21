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



// 假设上面的 automatic_debugging 函数已经定义

int main()
{
    PlayerService service;
    // 1. 玩家注册和操作示例
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
        // 登录/注册逻辑

        service.quickRegister();
        string playername, password;
        cout << "输入玩家姓名:" << endl;
        cin >> playername;
        cout << "输入密码:" << endl;
        cin >> password;

        // 验证登录
        service.check_and_login(playername, password);
        Player* player1 = service.getCurrentPlayer();
        service.check_and_logout();

        //4.测试每一次每一个玩家的计时
        cout << "输入玩家姓名:" << endl;
        cin >> playername;
        cout << "输入密码:" << endl;
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
        std::cerr << "\n错误: " << e.what() << std::endl;
        return 1;
    }
    return 0;

    
}
/*
我们的账号管理系统的使用方法如下案例：
介于我们的管理系统是基于页面开发的，里面的一些函数的传入数据类型就不能是const string了，所以在先前的函数定义有所修改
player1
123456

player1
123456
案例一：
    try
    {
        // 1. 玩家注册和操作示例
        std::cout << "\n===== 玩家注册 =====" << std::endl;

        // 2. 使用 PlayerService 类进行操作
        PlayerService service;
        std::cout << "\n===== 使用 PlayerService 类 =====" << std::endl;
        service.show();

        // 登录/注册逻辑
        string playername, password;
        cout << "输入玩家姓名:" << endl;
        cin >> playername;
        cout << "输入密码:" << endl;
        cin >> password;

        // 验证登录
        service.check_and_login(playername,password);
        Player* player1 = service.getCurrentPlayer();
        player1.setmode("hard");
        cout << "输入最好成绩(困难):" << endl;
        double bst;
        cin >> bst;
        player1->setBestScore(bst);
        service.check_and_logout();
        // 3. 获取所有玩家信息并排序
        std::cout << "\n===== 获取所有玩家信息并排序 =====" << std::endl;
        auto allPlayers = get_the_info();

        for (int i = 0; i < 3; i++)
        {
            cout << allPlayers[i] << endl;
        }
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n错误: " << e.what() << std::endl;
		return 1;
	}

    案例二：
        try
    {
        PlayerService service;
        // 1. 玩家注册和操作示例
        std::cout << "\n===== 玩家注册 =====" << std::endl;
        for (int i = 0; i < 3; i++)
        {
            string name1, pass1;
            cin >> name1;
            cin >> pass1;
            service.registerPlayer(name1, pass1);
        }
        // 2. 使用 PlayerService 类进行操作

        std::cout << "\n===== 使用 PlayerService 类 =====" << std::endl;
        service.show();

        // 登录/注册逻辑
        string playername, password;
        cout << "输入玩家姓名:" << endl;
        cin >> playername;
        cout << "输入密码:" << endl;
        cin >> password;

        // 验证登录
        service.check_and_login(playername, password);
        Player* player1 = service.getCurrentPlayer();
        cout << "输入最好成绩(困难):" << endl;
        double bst;
        cin >> bst;
        player1->setBestScore(bst, "hard");
        service.check_and_logout();
        // 3. 获取所有玩家信息并排序
        std::cout << "\n===== 获取所有玩家信息并排序 =====" << std::endl;
        auto allPlayers = get_the_info();

        for (int i = 0; i < 3; i++)
        {
            cout << allPlayers[i] << endl;
        }
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n错误: " << e.what() << std::endl;
        return 1;
    }

*/

/*
播放音乐的程序演示在下面

这个是关于如果我们想要点击喇叭按钮就自动来暂停和播放的功能的简单实现，真正实现的话需要和gui绑定
注意哈，下面出现的this_thread 表示的是我们的主线程，因为我们音频的播放是在另外的线程上实现的，不和主线程冲突!

  // 创建两个不同用途的播放器
    auto bgmPlayer = AudioPlayerFactory::createPlayer("背景音乐");
    auto battlePlayer = AudioPlayerFactory::createPlayer("排位音乐");

    // 加载对应目录的歌单文件
    // 背景音乐歌单路径：./bgm-playing/bgm.txt
    if (!bgmPlayer->load_playlist("./bgm-playing/bgm.txt")) {
        cout << "背景音乐歌单加载失败，程序退出" << endl;
        return 1;
    }

    // 排位音乐歌单路径：./bgm-ui/battle.txt
    if (!battlePlayer->load_playlist("./bgm-ui/bgm.txt")) {
        cout << "排位音乐歌单加载失败，程序退出" << endl;
        return 1;
    }

    // ---------------------- 播放器控制逻辑 ---------------------- //

    // 启动背景音乐
    bgmPlayer->play();
    cout << "背景音乐已启动..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // 播放5秒  

    // 启动排位音乐（与背景音乐同时播放）
    battlePlayer->play();
    cout << "排位音乐已启动..." << endl;
    this_thread::sleep_for(chrono::seconds(3)); // 同时播放3秒

    // 暂停背景音乐
    bgmPlayer->pause();
    cout << "背景音乐已暂停..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // 暂停状态保持5秒

    // 恢复背景音乐播放
    bgmPlayer->resume();
    cout << "背景音乐已恢复..." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // 恢复播放5秒

    // 停止所有正在播放的音乐
    bgmPlayer->stop();
    battlePlayer->stop();
    cout << "所有音乐已停止..." << endl;

    // ---------------------- 单曲循环示例 ---------------------- //

    // 创建新的单曲循环播放器（注意名称需唯一，此处使用"菜单音乐"避免重复）
    auto menuPlayer = AudioPlayerFactory::createPlayer("菜单音乐");

    // 设置单曲循环文件（假设文件路径正确）
    if (menuPlayer->set_single_music("./bgm-ui/qualifying_bgm.mp3")) {
        menuPlayer->set_single_loop(true); // 开启单曲循环模式
        menuPlayer->play();
        cout << "单曲循环已启动..." << endl;
        this_thread::sleep_for(chrono::seconds(10)); // 播放10秒
        menuPlayer->stop();
        cout << "单曲循环已停止..." << endl;
    }
    else {
        cout << "单曲文件加载失败" << endl;
    }

    // ---------------------- 资源清理 ---------------------- //

    // 删除所有播放器（可选，析构函数会自动清理资源）
    AudioPlayerFactory::deletePlayer("背景音乐");
    AudioPlayerFactory::deletePlayer("排位音乐");
    AudioPlayerFactory::deletePlayer("菜单音乐");

    cout << "程序执行完毕" << endl;
    return 0;
*/

/*
* 下面是提示报错的底层逻辑的演示，给了10个测试样例
* 注意返回的数据类型是string的，然后呢这个可以直接呈现在我们的报错提示栏里面的.
    // 测试用例1：正确的数独
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

    // 测试用例2：第一行有重复数字3
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

    // 测试用例3：第一列有重复数字5
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

    // 测试用例4：主对角线有重复数字5
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

    // 测试用例5：副对角线有重复数字2
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

    // 测试用例6：第一行和为46（错误）
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

    // 测试用例7：包含无效数字10
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

    // 测试用例8：部分填充但无错误
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

    // 测试用例9：部分填充且有重复数字
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

    // 测试用例10：主对角线已填满但和错误
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

    // 执行测试
    vector<vector<vector<int>>> tests = { test1, test2, test3, test4, test5, test6, test7, test8, test9, test10 };

    for (int i = 0; i < tests.size(); i++)
    {
        cout << "测试用例 " << i + 1 << " 的结果:" << endl;
        cout << automatic_debugging(tests[i]) << endl;
        cout << "----------------------------------------" << endl;
    }

    return 0;
*/

