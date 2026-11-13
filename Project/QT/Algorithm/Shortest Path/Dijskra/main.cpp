#include <bits/stdc++.h>
#include <QVector>
#include <queue>
#define INF 0x3F3F3F3F
#define pii pair<int, int>
using namespace std;
int n, m, s, e;
QVector<int> dis;
void Dijskra(int s, QVector<QVector<pii> >& edge) {
    QVector<bool> vis(n + 1);
    dis[s] = 0;
    priority_queue<pii, vector<pii>, greater<pii> > pq;
    pq.push({0, s});
    while (pq.size()) {
        int t = pq.top().second;
        pq.pop();
        if (vis[t]) continue;
        for (auto& i : edge[t]) {
            int v = i.first, w = i.second;
            if (dis[v] > dis[t] + w) {
                dis[v] = dis[t] + w;
                pq.push({dis[v], v});
            }
        }
    }
}
int main() {
    cin >> n >> m >> s >> e;
    int u, v, w;
    QVector<QVector<pii> > edge(n + 1);
    dis = QVector<int> (n + 1, INF);
    for (int i = 1; i <= m; i++) {
        cin >> u >> v >> w;
        edge[u].push_back({v, w});
    }
    Dijskra(s, edge);
    cout << dis[e];
    return 0;
}
