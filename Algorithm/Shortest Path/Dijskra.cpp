#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
int n, m, s, u, v, w;
vector<vector<pii>> edge;
vector<int> dis;
void Dijskra(int s) {
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    vector<bool> vis(n + 1);
    dis[s] = 0; pq.push({0, s});
    while (pq.size()) {
        int sn = pq.top().second; pq.pop();
        if (vis[sn]) continue;
        vis[sn] = 1;
        for (auto [fn, w] : edge[sn]) {
            if (dis[sn] + w < dis[fn]) {
                dis[fn] = dis[sn] + w;
                pq.push({dis[fn], fn});
            }
        }
    }
}
int main() {
    cin >> n >> m >> s;
    edge.resize(n + 1);
    dis = vector<int> (n + 1, INT_MAX);
    while (m--) {
        cin >> u >> v >> w;
        edge[u].push_back({v, w});
    }
    Dijskra(s);
    for (int i = 1; i <= n; i++) cout << dis[i] << " ";
    system("pause");
    return 0;
}