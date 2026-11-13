#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n, totalblock = 0, chromeblock = 0;
    cin >> m >> n;
    vector<int> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
    vector<vector<char>> grid(m, vector<char> (n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    vector<vector<char>> color = grid;
    function<void(int, int, char)> dfs = [&](int x, int y, char target) -> void {
        color[x][y] = '*';
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d];
            if (mx < 0 || mx == m || my < 0 || my == n || color[mx][my] != target) continue;
            dfs(mx, my, target);
        }
    };
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'G') grid[i][j] = 'B';
            if (color[i][j] == '*') continue;
            dfs(i, j, color[i][j]);
            totalblock++;
        }
    }
    color = grid;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (color[i][j] == '*') continue;
            dfs(i, j, color[i][j]);
            chromeblock++;
        }
    }
    cout << totalblock - chromeblock << endl;
    system("pause");
    return 0;
}