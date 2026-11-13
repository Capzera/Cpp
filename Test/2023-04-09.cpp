#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    int mod = 1e9 + 7;
    vector<int> dp(n + 1);
    if (n == 1) cout << 1 << endl;
    if (n == 2) cout << 2 << endl;
    if (n == 3) cout << 4 << endl;
    dp[1] = 1, dp[2] = 2, dp[3] = 4;
    for (int i = 4; i <= n; i++) {
        dp[i] = (dp[i - 1] + dp[i - 2] + dp[i - 3]) % mod;
    }
    cout << dp[n] << endl;
    system("pause");
    return 0;
}
/*
1 2 3 6 11 20 37 68 125
1 1 1 1
1 1 2
1 2 1
2 1 1
2 2
3 1
1 3
*/