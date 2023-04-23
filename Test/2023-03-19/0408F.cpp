#include <bits/stdc++.h>
using namespace std;
int m, n, flag;
int dx[4] = {0, 0, 1, -1}, dy[4] = {1, -1, 0, 0};
void dfs(int x, int y, vector<string>& grid) {
    grid[x][y] = '0';
    for (int d = 0; d < 4; d++) {
        int mx = x + dx[d], my = y + dy[d];
        if (mx < 0 || mx == m || my < 0 || my == n || grid[mx][my] != '1') continue;
        dfs(mx, my, grid);
    }
}
void dfs2(int x, int y, vector<string>& grid) {
    if (grid[x][y] == '1') return;
    grid[x][y] = '2';
    for (int d = 0; d < 4; d++) {
        int mx = x + dx[d], my = y = dy[d];
        if (mx < 0 || mx == m || my < 0 || my == n) {
            flag = 0;
            continue;
        }
        dfs2(mx, my, grid);
    }
}
void solve() {
    cin >> m >> n;
    int total = 0, circle = 0;
    vector<string> grid(m), backup;
    for (int i = 0; i < m; i++)
            cin >> grid[i];
    backup = grid;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            if (backup[i][j] == '0') continue;
            total++;
            dfs(i, j, backup);
        }
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '0') continue;
            grid[i][j] = '2';
            flag = 1;
            dfs2(i, j, grid);
            circle += flag;
        }
    cout << total - circle << endl;
}
int main() {
    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    system("pause");
    return 0;
}
/*
2
5 5
01111
11001
10101
10001
11111
5 6
111111
100001
010101
100001
111111
*/