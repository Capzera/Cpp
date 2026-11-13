#include <iostream>
#include <QDebug>
#include <QVector>
#define INF 0X3F3F3F3F

using namespace std;

int n, m, s, e;
QVector<QVector<int> > dp;
void Floyd() {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++){
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
            }
        }
    }
}
int main() {
    cin >> n >> m >> s >> e;
    int u, v, w;
    dp = QVector<QVector<int> > (n + 1, QVector<int> (n + 1, INF));
    for (int i = 1; i <= n; i++) dp[i][i] = 0;
    for (int i = 1; i <= m; i++) {
        cin >> u >> v >> w;
        dp[u][v] = w;
    }
    Floyd();
    if (dp[s][e] > INF) cout << "No Answer!";
    else cout << dp[s][e];
    return 0;
}
/*
4 5 1 4
1 3 4
1 2 1
3 4 1
1 4 6
2 4 2
*/
