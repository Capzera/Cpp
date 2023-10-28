#include <bits/stdc++.h>
#define pii pair<int, int>
#define tii tuple<int, int, int>
using namespace std;
const int INF = 0x3f3f3f3f;
vector<vector<pii>> edge;
vector<vector<int>> dis;
vector<vector<bool>> vis;
int n, m, k;
void Dijskra() {
    priority_queue<tii, vector<tii>, greater<tii> > pq;
    dis[1][0] = 0; pq.push({0, 1, 0});
    while (pq.size()) {
        auto [last, sn, x] = pq.top(); pq.pop();
        if (vis[sn][x]) continue;
        vis[sn][x] = 1;
        for (auto& [fn, limit] : edge[sn]) {
            int time = dis[sn][x], y = (x + 1) % k;
            if (time < limit) time += (limit - time + k - 1) / k * k;
            if (time + 1 < dis[fn][y]) {
                dis[fn][y] = time + 1;
                pq.push({dis[fn][y], fn, y});
            }
        }
    }
    if (dis[n][0] == INF) dis[n][0] = -1;
}
int main() {
    cin >> n >> m >> k;
    edge.resize(n + 1);
    dis = vector<vector<int> >(n + 1, vector<int>(k + 1, INF));
    vis = vector<vector<bool> > (n + 1, vector<bool>(k + 1));
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        edge[u].push_back({v, w});
    }
    Dijskra();
    cout << dis[n][0] << endl;
    system("pause");
    return 0;
}