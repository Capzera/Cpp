#include <bits/stdc++.h>
#include <QVector>
#define INF 0x3F3F3F3F

using namespace std;

struct Node {
    int v, w;
};
int n, m, s, e;
QVector<int> dis;

bool SPFA(int s, QVector<QVector<Node> >& edge) {
    QVector<bool> vis(n + 1);
    QVector<int> cnt(n + 1);
    dis = QVector<int> (n + 1, INF);
    dis[s] = 0, vis[s] = 1;
    queue<int> q;
    q.push(s);
    while (q.size()) {
        int t = q.front();
        q.pop();
        vis[t] = false;
        for (auto& i : edge[t]) {
            int v = i.v, w = i.w;
            if (dis[v] > dis[t] + w) {
                dis[v] = dis[t] + w;
                cnt[v] = cnt[t] + 1;
                if (cnt[v] == n) return 1;
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
    }
    return 0;
}

int main() {
    cin >> n >> m >> s >> e;
    int u, v, w;
    QVector<QVector<Node> > edge(n + 1);
    for (int i = 1; i <= m; i++) {
        cin >> u >> v >> w;
        edge[u].push_back({v, w});
    }
    if (SPFA(s, edge)) cout <<"No Answer!";
    else cout << dis[e] << endl;
    return 0;
}
/*
4 5 1 4
1 3 4
1 2 1
3 4 1
1 4 6
2 4 2
edge[1][0].second 2 3 4
edge[2] 4
edge[3] 4
*/
