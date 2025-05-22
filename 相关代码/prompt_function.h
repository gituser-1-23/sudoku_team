#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;
// 我们想要实现的功能是想pycharm一样的报错提示，变色...
// 第一个功能就是检查一行(列）里面是否出现了两个或以上的同样数字
// 第二个功能是当有满行或者满列，满斜的时候，是否和相等
// 第三个功能是玩家主动寻求帮助，那么我们提示的是目前所指行附近2x2或者3x3的范围内的数字

string automatic_debugging(vector<vector<int>> input_suduko)
{
    string error_messages = "";
    int expected_sum = 45; // 9x9数独每行/列/对角线的和应为45

    // 检查每行
    for (int i = 0; i < 9; i++) {
        vector<bool> seen(10, false); // 索引0-9，忽略索引0
        int sum = 0;
        bool has_duplicate = false;

        for (int j = 0; j < 9; j++) {
            int num = input_suduko[i][j];
            if (num == 0) continue; // 跳过未填入的单元格

            if (num < 1 || num > 9) {
                error_messages += "第 " + to_string(i + 1) + " 行包含无效数字 " + to_string(num) + "\n";
            }
            else {
                sum += num;
                if (seen[num]) {
                    has_duplicate = true;
                    error_messages += "第 " + to_string(i + 1) + " 行中数字 " + to_string(num) + " 重复出现\n";
                }
                seen[num] = true;
            }
        }

        // 只有当该行已填满（没有0）时才检查和
        if (!has_duplicate && sum != 0 && sum != expected_sum) {
            error_messages += "第 " + to_string(i + 1) + " 行的和为 " + to_string(sum) + "，不等于预期的 " + to_string(expected_sum) + "\n";
        }
    }

    // 检查每列
    for (int j = 0; j < 9; j++) {
        vector<bool> seen(10, false); // 索引0-9，忽略索引0
        int sum = 0;
        bool has_duplicate = false;

        for (int i = 0; i < 9; i++) {
            int num = input_suduko[i][j];
            if (num == 0) continue; // 跳过未填入的单元格

            if (num < 1 || num > 9) {
                error_messages += "第 " + to_string(j + 1) + " 列包含无效数字 " + to_string(num) + "\n";
            }
            else {
                sum += num;
                if (seen[num]) {
                    has_duplicate = true;
                    error_messages += "第 " + to_string(j + 1) + " 列中数字 " + to_string(num) + " 重复出现\n";
                }
                seen[num] = true;
            }
        }

        // 只有当该列已填满（没有0）时才检查和
        if (!has_duplicate && sum != 0 && sum != expected_sum) {
            error_messages += "第 " + to_string(j + 1) + " 列的和为 " + to_string(sum) + "，不等于预期的 " + to_string(expected_sum) + "\n";
        }
    }

    // 检查主对角线
    vector<bool> seen_main(10, false); // 索引0-9，忽略索引0
    int main_sum = 0;
    bool main_has_duplicate = false;
    bool main_has_zero = false;

    for (int i = 0; i < 9; i++) {
        int num = input_suduko[i][i];
        if (num == 0) {
            main_has_zero = true;
            continue; // 跳过未填入的单元格
        }

        if (num < 1 || num > 9) {
            error_messages += "主对角线包含无效数字 " + to_string(num) + "\n";
        }
        else {
            main_sum += num;
            if (seen_main[num]) {
                main_has_duplicate = true;
                error_messages += "主对角线中数字 " + to_string(num) + " 重复出现\n";
            }
            seen_main[num] = true;
        }
    }

    // 只有当主对角线已填满（没有0）时才检查和
    if (!main_has_duplicate && !main_has_zero && main_sum != expected_sum) {
        error_messages += "主对角线的和为 " + to_string(main_sum) + "，不等于预期的 " + to_string(expected_sum) + "\n";
    }

    // 检查副对角线
    vector<bool> seen_anti(10, false); // 索引0-9，忽略索引0
    int anti_sum = 0;
    bool anti_has_duplicate = false;
    bool anti_has_zero = false;

    for (int i = 0; i < 9; i++) {
        int num = input_suduko[i][8 - i];
        if (num == 0) {
            anti_has_zero = true;
            continue; // 跳过未填入的单元格
        }

        if (num < 1 || num > 9) {
            error_messages += "副对角线包含无效数字 " + to_string(num) + "\n";
        }
        else {
            anti_sum += num;
            if (seen_anti[num]) {
                anti_has_duplicate = true;
                error_messages += "副对角线中数字 " + to_string(num) + " 重复出现\n";
            }
            seen_anti[num] = true;
        }
    }

    // 只有当副对角线已填满（没有0）时才检查和
    if (!anti_has_duplicate && !anti_has_zero && anti_sum != expected_sum) {
        error_messages += "副对角线的和为 " + to_string(anti_sum) + "，不等于预期的 " + to_string(expected_sum) + "\n";
    }

    if (error_messages.empty()) {
        error_messages = "数独格式正确，没有发现错误";
    }

    return error_messages;
}