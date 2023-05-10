#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k;
    cin >> n >> k;
    vector<int> dp(k + 1);
    dp[1] = 2;
    for (int i = 3; i <= n; ++i) {
        for (int j = k; j >= 2; --j)
            dp[j] = (dp[j] * j + dp[j - 1] * 2 + dp[j - 2] * (i - j)) % 123456;
    }
    cout << dp[k];
    return 0;
} 