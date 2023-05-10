#include <bits/stdc++.h>
using namespace std;
int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int solve() {
    int n, k;
    cin >> n >> k;
    vector<string> grid(n);
    vector<vector<vector<int>>> dis(n, vector<vector<int>> (n, vector<int>(3, -1)));
    for (int i = 0; i < n; i++) cin >> grid[i];
    queue<tuple<int, int, int>> q;
    dis[2][2][2] = 0, q.push({2, 2, 2});
    while (q.size()) {
        auto [x, y, r] = q.front(); q.pop();
        int time = dis[x][y][r], R = max(0, 2 - (time + 1) / k);
        if (x == n - 3 && y == n - 3) return time;
        if (r) {
            q.push({x, y, R});
            dis[x][y][R] = time + 1;
        }
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d], flag = 1;
            if (mx - r < 0 || mx + r >= n || my - r < 0 || my + r >= n) continue;//ÅÐÔ½½ç
            if (dis[mx][my][r] != -1) continue;
            for (int i = mx - r; flag && i <= mx + r; i++)
                for (int j = my - r; flag && j <= my + r; j++)
                    if (grid[i][j] == '*') 
                        flag = 0;
            if (!flag) continue;
            q.push({mx, my, R});
            dis[mx][my][R] = time + 1;
        }
    }
    return -1;
}
int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}