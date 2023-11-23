#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int n, first = -1, ans = 0, x;
    cin >> n;
    vector<vector<int> > grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cin >> grid[i][j];
            grid[j][i] = grid[i][j];
        }
    }
    for (auto& g : grid) {
        sort(g.begin(), g.end(), greater<int>());
        ans = max(ans, g[1]);
    }
    cout << 1 << endl << ans << endl;
    system("pause");
    return 0;
}
