#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<vector<int>> grid(n, vector<int> (n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) cin >> grid[i][j];
    long long ans = 0;
    for (int i = 0; i < n; i++) ans += grid[0][i];
    for (int i = 1; i < n; i++) ans += grid[i][0] + grid[i][n - 1];
    for (int i = 1; i < n - 1; i++) ans += grid[n - 1][i];
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
3
-3 2 0
1 2 3
-3 -2 -1
*/