#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int m, n;
    cin >> m >> n;
    int dx[3] = {0, 1, 1}, dy[3] = {1, 0, 1};
    vector<string> grid(m);
    queue<pair<int, int> > q;
    for (int i = 0; i < m; i++) {
        cin >> grid[i];
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '#') q.push({i, j});
        }
    }
    while (q.size()) {
        auto [x, y] = q.front(); q.pop();
        for (int d = 0; d < 3; d++) {
            int mx = x + dx[d], my = y + dy[d];
            if (mx < 0 || mx == m || my < 0 || my == n || grid[mx][my] == '#') continue;
            grid[mx][my] = '#';
        }
    }
    for (int i = 0; i < m; i++) cout << grid[i] << endl;
    return 0;
}