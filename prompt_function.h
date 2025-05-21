#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;
// ������Ҫʵ�ֵĹ�������pycharmһ���ı�����ʾ����ɫ...
// ��һ�����ܾ��Ǽ��һ��(�У������Ƿ���������������ϵ�ͬ������
// �ڶ��������ǵ������л������У���б��ʱ���Ƿ�����
// �������������������Ѱ���������ô������ʾ����Ŀǰ��ָ�и���2x2����3x3�ķ�Χ�ڵ�����

string automatic_debugging(vector<vector<int>> input_suduko)
{
    string error_messages = "";
    int expected_sum = 45; // 9x9����ÿ��/��/�Խ��ߵĺ�ӦΪ45

    // ���ÿ��
    for (int i = 0; i < 9; i++) {
        vector<bool> seen(10, false); // ����0-9����������0
        int sum = 0;
        bool has_duplicate = false;

        for (int j = 0; j < 9; j++) {
            int num = input_suduko[i][j];
            if (num == 0) continue; // ����δ����ĵ�Ԫ��

            if (num < 1 || num > 9) {
                error_messages += "�� " + to_string(i + 1) + " �а�����Ч���� " + to_string(num) + "\n";
            }
            else {
                sum += num;
                if (seen[num]) {
                    has_duplicate = true;
                    error_messages += "�� " + to_string(i + 1) + " �������� " + to_string(num) + " �ظ�����\n";
                }
                seen[num] = true;
            }
        }

        // ֻ�е�������������û��0��ʱ�ż���
        if (!has_duplicate && sum != 0 && sum != expected_sum) {
            error_messages += "�� " + to_string(i + 1) + " �еĺ�Ϊ " + to_string(sum) + "��������Ԥ�ڵ� " + to_string(expected_sum) + "\n";
        }
    }

    // ���ÿ��
    for (int j = 0; j < 9; j++) {
        vector<bool> seen(10, false); // ����0-9����������0
        int sum = 0;
        bool has_duplicate = false;

        for (int i = 0; i < 9; i++) {
            int num = input_suduko[i][j];
            if (num == 0) continue; // ����δ����ĵ�Ԫ��

            if (num < 1 || num > 9) {
                error_messages += "�� " + to_string(j + 1) + " �а�����Ч���� " + to_string(num) + "\n";
            }
            else {
                sum += num;
                if (seen[num]) {
                    has_duplicate = true;
                    error_messages += "�� " + to_string(j + 1) + " �������� " + to_string(num) + " �ظ�����\n";
                }
                seen[num] = true;
            }
        }

        // ֻ�е�������������û��0��ʱ�ż���
        if (!has_duplicate && sum != 0 && sum != expected_sum) {
            error_messages += "�� " + to_string(j + 1) + " �еĺ�Ϊ " + to_string(sum) + "��������Ԥ�ڵ� " + to_string(expected_sum) + "\n";
        }
    }

    // ������Խ���
    vector<bool> seen_main(10, false); // ����0-9����������0
    int main_sum = 0;
    bool main_has_duplicate = false;
    bool main_has_zero = false;

    for (int i = 0; i < 9; i++) {
        int num = input_suduko[i][i];
        if (num == 0) {
            main_has_zero = true;
            continue; // ����δ����ĵ�Ԫ��
        }

        if (num < 1 || num > 9) {
            error_messages += "���Խ��߰�����Ч���� " + to_string(num) + "\n";
        }
        else {
            main_sum += num;
            if (seen_main[num]) {
                main_has_duplicate = true;
                error_messages += "���Խ��������� " + to_string(num) + " �ظ�����\n";
            }
            seen_main[num] = true;
        }
    }

    // ֻ�е����Խ�����������û��0��ʱ�ż���
    if (!main_has_duplicate && !main_has_zero && main_sum != expected_sum) {
        error_messages += "���Խ��ߵĺ�Ϊ " + to_string(main_sum) + "��������Ԥ�ڵ� " + to_string(expected_sum) + "\n";
    }

    // ��鸱�Խ���
    vector<bool> seen_anti(10, false); // ����0-9����������0
    int anti_sum = 0;
    bool anti_has_duplicate = false;
    bool anti_has_zero = false;

    for (int i = 0; i < 9; i++) {
        int num = input_suduko[i][8 - i];
        if (num == 0) {
            anti_has_zero = true;
            continue; // ����δ����ĵ�Ԫ��
        }

        if (num < 1 || num > 9) {
            error_messages += "���Խ��߰�����Ч���� " + to_string(num) + "\n";
        }
        else {
            anti_sum += num;
            if (seen_anti[num]) {
                anti_has_duplicate = true;
                error_messages += "���Խ��������� " + to_string(num) + " �ظ�����\n";
            }
            seen_anti[num] = true;
        }
    }

    // ֻ�е����Խ�����������û��0��ʱ�ż���
    if (!anti_has_duplicate && !anti_has_zero && anti_sum != expected_sum) {
        error_messages += "���Խ��ߵĺ�Ϊ " + to_string(anti_sum) + "��������Ԥ�ڵ� " + to_string(expected_sum) + "\n";
    }

    if (error_messages.empty()) {
        error_messages = "������ʽ��ȷ��û�з��ִ���";
    }

    return error_messages;
}