#include <bits/stdc++.h>
using namespace std;
long long solve(int m, int n) {
    vector<int> st(1 << m);
    for (int mask = 0; mask < 1 << m; ++mask) {
        int cnt = 0;
        st[mask] = 1;
        for (int i = 0; i < m; ++i) {
            if (mask >> i & 1) {
                if (cnt & 1) st[mask] = 0;
            }
            else cnt++;
        }
        if (cnt & 1) st[mask] = 0;
    }
    vector<vector<long long>> dp(n + 1, vector<long long>(1 << m));
    dp[0][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j < 1 << m; ++j)
            for (int k = 0; k < 1 << m; ++k) {
                if (j & k || !st[j | k]) continue;
                dp[i][j] += dp[i - 1][k];
            }
    return dp[n][0];
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int m, n;
    while (cin >> m >> n, m || n){
        cout << solve(m, n) << endl;
    }
    system("pause");
    return 0;
}