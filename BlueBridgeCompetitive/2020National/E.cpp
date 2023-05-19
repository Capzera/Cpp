#include <bits/stdc++.h>
using namespace std;
int main() {
    const int n = 4;
    int ans = 0, dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
    vector<vector<int> > vis(n, vector<int>(n));
    function<void(int, int, int)> dfs = [&](int i, int x, int y) {
        if (i == 16) {
            ans++;
            return;
        }
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d];
            if (mx < 0 || mx >= 4 || my < 0 || my >= 4 || vis[mx][my]) continue;
            vis[mx][my] = 1;
            dfs(i + 1, mx, my);
            vis[mx][my] = 0;
        }
    };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            vis[i][j] = 1;
            dfs(1, i, j);
            vis[i][j] = 0;
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
题目描述
本题为填空题，只需要算出结果后，在代码中使用输出语句将所填结果输出即可。
小蓝有一条玩具蛇，一共有 16 节，上面标着数字 1 至 16。每一节都是一个正方形的形状。相邻的两节可以成直线或者成 90 度角。
小蓝还有一个 4×4 的方格盒子，用于存放玩具蛇，盒子的方格上依次标着字母 A 到 P 共 16 个字母。
小蓝可以折叠自己的玩具蛇放到盒子里面。他发现，有很多种方案可以将玩具蛇放进去。
下图给出了两种方案：
1   2   3   4 
8   7   6   5
9   10  11  12
16  15  14  13

13 12 11 10
14 01 02 09
15 04 03 08
16 05 06 07
请帮小蓝计算一下，总共有多少种不同的方案。如果两个方案中，存在玩具蛇的某一节放在了盒子的不同格子里，则认为是不同的方案。
*/