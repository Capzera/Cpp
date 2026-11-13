#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int INF = 0x3f3f3f3f;
int n, m, k, tot = 0;
vector<int> dis, nxt, ver, edge, head;
vector<bool> vis;
void add(int x, int y, int z) {
    ver[++tot] = y;
    edge[tot] = z;
    nxt[tot] = head[x];
    head[x] = tot;
}

void Dijskra(int s) {
    priority_queue<pii> pq;
    dis[s] = 0; pq.push({0, s});
    while (pq.size()) {
        int node = pq.top().second; pq.pop();
        if (vis[node]) continue;
        vis[node] = 1;
        for (int i = head[node]; i; i = nxt[i]) {
            int y = ver[i], z = edge[i];
            int t = dis[node] + 1;
            if (t < z) t += ((z - dis[node] + k - 1) / k) * k;
            if (t < dis[y]) {
                dis[y] = t;
                pq.push({-dis[y], y});
            }
        }
    }
}

int idx(int layer, int x) {
    return layer * n + x;
}

int main() {
    cin >> n >> m >> k;
    nxt = head = edge = ver = vector<int>(1e6 + 5);
    dis = vector<int>(n + 1, INF);
    vis.resize(n + 1);
    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        for (int j = 0; j < k; j++) {
            int curIdx = idx(j, u);
            int nxtIdx = idx((j + 1) % k, v);
            add(curIdx, nxtIdx, w);
        }
    }
    Dijskra(1);
    if (dis[n] == INF) cout << -1;
    else cout << dis[n];
    system("pause");
    return 0;
}
/*
5 5 3
1 2 0
2 5 1
1 3 0
3 4 3
4 5 1
*/