#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n, ans = 0;
    cin >> m >> n;
    vector<string> grid(m);
    int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
    for (auto& x : grid) cin >> x;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (auto k = '1'; k <= grid[i][j]; k++) {
                int cnt = 4 + (k == '1') + (k == grid[i][j]);
                for (int d = 0; d < 4; d++) {
                    int x = i + dx[d], y = j + dy[d];
                    if (x < 0 || x == m || y < 0 || y == n) continue;
                    cnt -= (grid[x][y] >= k);
                }
                ans += cnt;
            }
        }
    }
    cout << ans << endl;
    system("pause");
    return 0;
}