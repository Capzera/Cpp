#include <iostream>
#include <QVector>
#include <QDebug>
#define INF 0x3F3F3F3F

using namespace std;

struct Node{
    int u, v, w;
};
QVector<Node> edge;
QVector<int> dis;
int n, m, s, e;

bool Bellman_Ford(int s) {
    dis = QVector<int> (n + 1, INF);
    dis[s] = 0;
    for (int i = 1; i <= n; i++) {//枚举轮数
        for (auto& j : edge) {//枚举所有的边
            int u = j.u, v = j.v, w = j.w;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (i == n) return 1;//如果第n轮依然在更新，到第n轮说明成环，第n轮还在更新，说明成负环
            }
        }
    }
    return 0;
}

int main() {
    cin >> n >> m >> s >> e;
    int u, v, w;
    for (int i = 1; i <= m; i++) {
        cin >> u >> v >> w;
        edge.push_back({u, v, w});
        //edge.push_back({v, u, w});
    }
    bool flag = Bellman_Ford(s);
    if (flag) cout << "No Answer";
    else cout << dis[e] << endl;
    return 0;
}
