#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <unordered_set>
#include <string>
using namespace std;
int main() {
    int cor = 0, tot = 0;
    srand(time(NULL));
    unordered_set<int> ps;  
    vector<string> pro = {
        "数据清洗方法不包括（）",
        "智能手环的应用开发，主要应用（）的数据采集技术。",
        "强关联规则，即这些规则必须满足最小支持度和（）。",
        "噪声数据处理的方法主要有（）。",
        "大数据环境下的隐私担忧，主要表现为（）。",
        "对线下零售而言，做好大数据分析应用的前提是（）",
        "（）是一个观测值，它与其他观测值的差别如此之大，以至于怀疑它是由不同的机制产生的。",
        "下面例子中不属于分类的是（）",
        "k近邻法的基本要素不包括（）。",
        "建立一个模型，并通过该模型根据已知的变量值来预测其他某个变量值，属于数据挖掘的哪一类任务？（）"
    }, sa = {
        "噪声数据清除",
        "网络爬虫",
        "最小兴趣度",
        "分类",
        "个人信息的被识别与暴露",
        "增加统计种类 ",
        "边界点",
        "检测图像中是否有人脸出现 ",
        "距离度量",
        "根据内容检索"
    }, sb = {
        "一致性检查 ",
        "API接口",
        "最大置信度",
        "聚类",
        "用户画像的生成",
        "扩大营业面积",
        "质心",
        "对客户按照贷款风险大小进行分类",
        "k值的选择",
        "建模描述"
    }, sc = {
        "重复数据记录处理",
        "传感器",
        "最大支持度",
        "回归",
        "恶意广告的推送",
        "增加数据来源 ",
        "离群点",
        "识别手写的数字",
        "样本大小",
        "预测建模"
    }, sd = {
        "缺失值处理",
        "统计报表",
        "最小置信度",
        "以上都有",
        "病毒入侵",
        "开展优惠促销",
        "核心点",
        "估计商场客流量",
        "分类决策规则",
        "寻找模式和规则"
    };
    vector<char> ans = {
        'C',
        'C',
        'D',
        'D',
        'A',
        'C',
        'C',
        'D',
        'C',
        'C'
    };
    char ch = 'z';
    auto check = [&](string cur, int id) {
        string str;
        if (ans[id] == 'A') str = sa[id];
        if (ans[id] == 'B') str = sb[id];
        if (ans[id] == 'C') str = sc[id];
        if (ans[id] == 'D') str = sd[id];
        return cur == str;
    };
    while (1) {
        int id = rand() % 10, ran;
        if (ps.size() == 10) ps.clear();
        while (ps.count(id)) {
            id = rand() % 10;
        }
        ps.emplace(id);
        cout << pro[id] << endl;
        unordered_set<int> s;
        while (s.size() < 4) {
            ran = rand() % 4;
            s.emplace(ran);
        }
        char num = 'A', an = 'z';
        for (auto & k : s) {
            cout << num << " ";
            if (k == 0) {
                cout << sa[id] << endl;
                if (check(sa[id], id)) an = num;
            }
            if (k == 1) {
                cout << sb[id] << endl;
                if (check(sb[id], id)) an = num;
            }
            if (k == 2) {
                cout << sc[id] << endl;
                if (check(sc[id], id)) an = num;
            }
            if (k == 3) {
                cout << sd[id] << endl;
                if (check(sd[id], id)) an = num;
            }

            num++;
        }
        cin >> ch;
        ch = toupper(ch);
        if (ch == 'Q') break;
        tot++;
        if (an == ch) {
            cout << "正确" << endl;
            cor++;
        }
        else {
            cout << "错误" << endl;
        }
        system("pause");
        system("cls");
    }
    system("cls");
    cout << "正确 " << tot << " 题 ";
    double x = 100.0 * cor / tot;
    printf("正确率为 %.2f%\n", x);
    system("pause");
    return 0;
}
