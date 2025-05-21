#include<iostream>
#include"solution_of_Suduko.h";
#include"players_num.h"
#include"create_players.h"
#include"scores_of_all_players.h"
#include"player_system.h"
#include"sort_the_players.h"
#include"bgm_audio_playing.h"
using namespace std;

// 初始化空数独棋盘
vector<vector<int>>empty1 =
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

int main()
{
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
}
/*
关于账号功能的设计部分：

#include"players_num.h"
#include"create_players.h"
    // 方法1：使用create_new_player函数自动创建
    create_new_player("./players/");  // 创建player1

    // 方法2：手动创建并保存
    Player custom("李俊昊", "54321", 100, 50, 20);
    custom.saveToFile("./players/");

    // 方法3：加载现有玩家
    Player loaded;
    loaded.loadFromFile("./players/李俊昊.txt");
    std::cout << "加载的玩家: " << loaded.getName() << std::endl;

    // 修改并保存
    loaded.reset("warrior_master");
    loaded.setScores(150, 80, 40);
    loaded.saveToFile("./players/");
    return 0;
*/

/*
我们的账号管理系统的使用方法如下案例：
介于我们的管理系统是基于页面开发的，里面的一些函数的传入数据类型就不能是const string了，所以在先前的函数定义有所修改
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
 if (!play("./bgm-ui/bgm.txt")) {
        cout << "初始化播放失败，程序退出" << endl;
        return 1;
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
    return 0;

    下面是单曲循环的播放方法:
int main()
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
    return 0;
}
*/

/*下面演示的是一个玩家的账号创建和信息的修改，其中的端口是setBestScore和incrementLevelCount
* 也就是说一局完成后，这一局的分数会被记录下来
    try
    {
        // 1. 创建并保存玩家对象
        std::cout << "\n===== 创建新玩家 =====" << std::endl;

        // 创建玩家1：使用默认构造函数（自动生成名称）
        Player player1;
        player1.setBestScore(120.5, "simple");  // 设置简单模式成绩
        player1.incrementLevelCount("simple");   // 简单模式通关+1
        player1.saveToFile();                    // 保存到默认路径 ./players/
        std::cout << "创建玩家: " << player1.getName() << std::endl;

        // 创建玩家2：指定名称和密码
        Player player2("Bob", "secure123");
        player2.setBestScore(85.0, "simple");    // 简单模式最佳成绩
        player2.setBestScore(150.0, "mid");   // 中等模式最佳成绩
        player2.incrementLevelCount("simple");    // 简单模式通关+1
        player2.incrementLevelCount("mid");    // 中等模式通关+1
        player2.saveToFile();
        std::cout << "创建玩家: " << player2.getName() << std::endl;

        // 创建玩家3：使用静态方法（自动生成名称）
        Player::createNewPlayer("./players/");    // 调用静态方法创建
        std::cout << "当前玩家总数: " << Player::getPlayerCount() << std::endl;

        // 2. 从文件加载玩家数据（以玩家2为例）
        std::cout << "\n===== 加载玩家信息 =====" << std::endl;
        Player loadedPlayer;
        std::string filePath = "./players/Bob.txt";

        if (loadedPlayer.loadFromFile(filePath)) {
            std::cout << "加载玩家: " << loadedPlayer.getName() << std::endl;
            std::cout << "简单模式成绩: " << loadedPlayer.getBestSimpleScore() << " 秒" << std::endl;
            std::cout << "中等模式通关次数: " << loadedPlayer.getMidModeCount() << std::endl;
        }
        else {
            std::cerr << "加载失败: " << filePath << std::endl;
        }

        // 3. 修改玩家信息并重新保存
        std::cout << "\n===== 修改玩家信息 =====" << std::endl;
        loadedPlayer.reset("Bob_Updated", "newPassword456");  // 修改名称和密码
        loadedPlayer.setBestScore(200.0, "hard");             // 设置困难模式成绩
        loadedPlayer.incrementLevelCount("hard");              // 困难模式通关+1
        loadedPlayer.saveToFile();                             // 保存修改后的数据

        std::cout << "修改后玩家名: " << loadedPlayer.getName() << std::endl;
        std::cout << "困难模式最佳成绩: " << loadedPlayer.getBestHardScore() << " 秒" << std::endl;

        // 4. 获取所有玩家信息并打印
        std::cout << "\n===== 所有玩家信息 =====" << std::endl;
        auto allPlayers = get_all_info();

        for (const auto& info : allPlayers) {
            std::cout << "\n玩家: " << info.name << std::endl;
            std::cout << "简单模式: " << (info.simpleTime < 999999.9 ?
                std::to_string(info.simpleTime) + " 秒" : "未完成") << std::endl;
            std::cout << "中等模式: " << (info.mediumTime < 999999.9 ?
                std::to_string(info.mediumTime) + " 秒" : "未完成") << std::endl;
            std::cout << "困难模式: " << (info.hardTime < 999999.9 ?
                std::to_string(info.hardTime) + " 秒" : "未完成") << std::endl;
        }

        // 5. 按各个模式排序并打印排名  这个只是例子，简单模式和中等模式的排序是一个道理
        std::cout << "\n===== 困难模式排名 =====" << std::endl;
        sort_the_players(allPlayers, GameMode::Hard);

        for (size_t i = 0; i < allPlayers.size(); ++i) {
            std::cout << "第 " << i + 1 << " 名: " << allPlayers[i].name
                << " - " << allPlayers[i].hardTime << " 秒" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\n错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/

