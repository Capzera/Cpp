#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<int> stones(n), sum(n + 1);
    vector<vector<int> > dp(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        cin >> stones[i];
        sum[i + 1] = sum[i] + stones[i];
    }
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1, val = sum[j + 1] - sum[i];
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + val);
            }
        }
    }
    cout << dp[0][n - 1] << endl;
    return 0;
}