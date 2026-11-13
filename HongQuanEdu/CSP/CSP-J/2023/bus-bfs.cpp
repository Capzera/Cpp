#include <bits/stdc++.h>
using namespace std;

int solve(int n, int k, vector<vector<int> >& edges) {
    vector<int> edge[n];
    vector<vector<int> > limit(n, vector<int>(n));
    for (auto& e : edges) {
        limit[e[0]][e[1]] = e[2];
        edge[e[0]].emplace_back(e[1]);
    }
    queue<pair<int, int>> q;
    q.push({1, k});
    while (q.size()) {
        auto& [sn, time] = q.front(); q.pop();
        for (auto& fn : edge[sn]) {
            if (fn == (n - 1) && (time + 1) % k == 0) return time + 1;
            if (time < limit[sn][fn]) q.push({fn, limit[sn][fn] + k - 1 + (time % k) + 1});
            else q.push({fn, time + 1});
        }
    }
    return -1;
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int> > edges(m, vector<int>(3));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) cin >> edges[i][j];
    }
    cout << solve(n + 1, k, edges) << endl;
    system("pause");
    return 0;
}