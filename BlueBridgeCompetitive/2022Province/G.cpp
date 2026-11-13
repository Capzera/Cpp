#include <bits/stdc++.h>
using namespace std;
int solve() {
    int n, mod = 1e9 + 7;
    cin >> n;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 5;
    vector<long long> dp(n + 1);
    dp[0] = dp[1] = 1;
    dp[2] = 2, dp[3] = 5;
    for (int i = 4; i <= n; i++) {
        dp[i] = (2 * dp[i - 1] + dp[i - 3]) % mod;
    }
    return dp[n];
}

int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}
/*
f[i] = f[i - 1] + f[i - 2] + 2 * f[i - 3] + 2 * f[i - 4] + 2 * f[i - 5] + бнбн + 2 * f[1];
f[i - 1] =        f[i - 2] + 1 * f[i - 3] + 2 * f[i - 4] + 2 * f[i - 5] + бнбн + 2 * f[1];
f[i] - f[i - 1] = f[i - 1] + f[i - 3]
f[i] = 2 * f[i - 1] + f[i - 3]
*/