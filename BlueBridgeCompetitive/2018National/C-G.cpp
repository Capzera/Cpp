#include <bits/stdc++.h>
using namespace std;

int solve() {
    int n, k;
    cin >> n >> k;
    vector<string> grid(n);
    vector<int> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
    for (int i = 0; i < n; i++) cin >> grid[i];
    vector<vector<vector<int>>> dis(n + 1, vector<vector<int>>(n + 1, vector<int>(k + 1, -1)));
    set<pair<int, int>> s;//s保存无敌道具
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == '%') s.insert({i, j});
    queue<tuple<int, int, int>> q;
    dis[0][0][0] = 0, q.push({0, 0, 0});
    while (q.size()) {
        auto [x, y, last] = q.front(); q.pop();
        if (x == n - 1 && y == n - 1) return dis[x][y][last];
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d];
            if (mx < 0 || mx == n || my < 0 || my == n || grid[mx][my] == '#') continue;
            if (!last && grid[mx][my] == 'X') continue;
            if (s.count({mx, my})) {//目标格是一个无敌道具格
                if (dis[mx][my][k] != -1) continue;
                dis[mx][my][k] = dis[x][y][last] + 1;
                q.push({mx, my, k});
                s.erase({mx, my});
            }
            else {
                int next = last ? last - 1 : 0;
                if (dis[mx][my][next] != -1) continue;
                dis[mx][my][next] = dis[x][y][last] + 1;
                q.push({mx, my, next});
            }
        }
    }
    return -1;
}
int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}
/*
迷宫与陷阱, 第九届C/C++ 本科C组 Problem G
小明在玩一款迷宫游戏，在游戏中他要控制自己的角色离开一间由 N \times NN×N 个格子组成的 2D 迷宫。
小明的起始位置在左上角，他需要到达右下角的格子才能离开迷宫。
每一步，他可以移动到上下左右相邻的格子中（前提是目标格子可以经过）。
迷宫中有些格子小明可以经过，我们用 '.' 表示。
有些格子是墙壁，小明不能经过，我们用 '#' 表示。
此外，有些格子上有陷阱，我们用 'X' 表示。除非小明处于无敌状态，否则不能经过。
有些格子上有无敌道具，我们用 '%' 表示。
当小明第一次到达该格子时，自动获得无敌状态，无敌状态会持续 KK 步。
之后如果再次到达该格子不会获得无敌状态了。
处于无敌状态时，可以经过有陷阱的格子，但是不会拆除/毁坏陷阱，即陷阱仍会阻止没有无敌状态的角色经过。
给定迷宫，请你计算小明最少经过几步可以离开迷宫?
*/