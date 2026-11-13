/*
又到暑假了，住在城市 A 的 Car 想和朋友一起去城市旅游。
她知道每个城市都有4个飞机场，分别位于一个矩形的4个顶点上，
同一个城市中两个机场之间有一条笔直的高速铁路，第i个城市中高速铁路了的单位里程价格为 Ti，
任意两个不同城市的机场之间均有航线，所有航线单位里程的价格均为t。
*/
#include <bits/stdc++.h>
using namespace std;
double solve() {
    int s, t, A, B;
    double ans = DBL_MAX;
    cin >> s >> t >> A >> B;
    vector<vector<double> > dis(4 * s, vector<double>(4 * s));
    vector<int> T(s), x, y;
    auto get = [&](int i, int j) -> double {
        return sqrt(1.0 * (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
    };
    for (int i = 0; i < s; i++) {
        int x0, x1, x2, x3, y0, y1, y2, y3;
        cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> T[i];
        x.push_back(x0), x.push_back(x1), x.push_back(x2);
        y.push_back(y0), y.push_back(y1), y.push_back(y2);
        if ((x2 - x0) * (x1 - x0) + (y2 - y0) * (y1 - y0) == 0) {//x0是直角点
            x3 = x1 + x2 - x0, y3 = y1 + y2 - y0;
        }
        if ((x2 - x1) * (x0 - x1) + (y2 - y1) * (y0 - y1) == 0) {//x1是直角点
            x3 = x0 + x2 - x1, y3 = y0 + y2 - y1;
        }
        if ((x1 - x2) * (x0 - x2) + (y1 - y2) * (y0 - y2) == 0) {//x2是直角点
            x3 = x0 + x1 - x2, y3 = y0 + y1 - y2;
        }
        x.emplace_back(x3), y.emplace_back(y3);
    }
    //初始化dis数组
    for (int i = 0; i < 4 * s; i++) {
        for (int j = 0; j < 4 * s; j++) {
            if (i == j) continue;
            //计算高速公路
            if (i / 4 == j / 4) dis[i][j] = T[i / 4] * get(i, j);
            //计算航线
            else dis[i][j] = t * get(i, j);
        }
    }

    //Floyd算法
    for (int k = 0; k < 4 * s; k++)
        for (int i = 0; i < 4 * s; i++)
            for (int j = 0; j < 4 * s; j++)
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);

    //求解答案
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ans = min(ans, dis[(A - 1) * 4 + i][(B - 1) * 4 + j]);
    return ans;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        cout << fixed << setprecision(1) << solve() << endl;
    }
    system("pause");
    return 0;
}
/*
1 3
0 2
0, 8
0 1 2 3
4 5 6 7
8 9 10 11
*/