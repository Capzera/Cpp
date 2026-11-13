/*
题目描述
小 Z 打算在国庆假期期间搭乘旅游巴士去一处他向往已久的景点旅游。
旅游景点的地图共有 n 处地点，在这些地点之间连有 m 条道路。
其中 1 号地点为景区入口，n 号地点为景区出口。我们把一天当中景区开门营业的时间记为 0 时刻，
则从 0 时刻起，每间隔 k 单位时间便有一辆旅游巴士到达景区入口，同时有一辆旅游巴士从景区出口驶离景区。
所有道路均只能单向通行。对于每条道路，游客步行通过的用时均为恰好 1 单位时间。
小 Z 希望乘坐旅游巴士到达景区入口，并沿着自己选择的任意路径走到景区出口，
再乘坐旅游巴士离开，这意味着他到达和离开景区的时间都必须是 k 的非负整数倍。
由于节假日客流众多，小 Z 在旅游巴士离开景区前只想一直沿着景区道路移动，而不想在任何地点（包括景区入口和出口）或者道路上停留。
出发前，小 Z 忽然得知：景区采取了限制客流的方法，对于每条道路均设置了一个“开放时间”a_i，游客只有不早于a_i时刻才能通过这条道路。
请帮助小 Z 设计一个旅游方案，使得他乘坐旅游巴士离开景区的时间尽量地早。
*/
#include <bits/stdc++.h>
#define pii pair<int, int>
#define tii tuple<int, int, int>
using namespace std;
vector<vector<pii>> edge;
vector<vector<int>> dis;
int n, m, k, u, v, w;
void Dijskra() {
    priority_queue<tii, vector<tii>, greater<tii> > pq;
    vector<vector<bool>> vis(n + 1, vector<bool>(k + 1));
    dis[1][0] = 0; pq.push({0, 1, 0});
    while (pq.size()) {
        auto [last, sn, x] = pq.top(); pq.pop();
        if (vis[sn][x]) continue;
        vis[sn][x] = 1;
        for (auto [fn, limit] : edge[sn]) {
            int time = dis[sn][x], y = (x + 1) % k;
            if (time < limit) time += (limit - time + k - 1) / k * k;
            if (time + 1 < dis[fn][y]) {
                dis[fn][y] = time + 1;
                pq.push({dis[fn][y], fn, y});
            }
        }
    }
    if (dis[n][0] == INT_MAX) dis[n][0] = -1;
}
int main() {
    cin >> n >> m >> k;
    edge.resize(n + 1);
    dis = vector<vector<int>>(n + 1, vector<int>(k + 1, INT_MAX));
    while (m--) {
        cin >> u >> v >> w;
        edge[u].push_back({v, w});
    }
    Dijskra();
    cout << dis[n][0] << endl;
    system("pause");
    return 0;
}