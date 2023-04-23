#include <bits/stdc++.h>
using namespace std;
void solve() {
    int m, n, k;
    vector<vector<int>> grid(m + 1, vector<int>(n + 1));
    for (int i = 0; i < k; i++) {
        int a, b;
        cin >> a >> b;
        grid[a][b] = 1;
    }
    int x0, y0, x1, y1;
    cin >> x0 >> y0 >> x1 >> y1;
    
}
int main() {
    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    system("pause");
    return 0;
}