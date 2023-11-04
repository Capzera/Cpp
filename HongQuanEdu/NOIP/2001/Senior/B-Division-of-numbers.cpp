/*
将整数 n 分成 k 份，且每份不能为空，任意两个方案不相同（不考虑顺序）。
例如 n=7，k=3，下面三种分法被认为是相同的。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k;
    cin >> n >> k;
    int dp[n + 1][k + 1];
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= min(i, k); j++)
            dp[i][j] = dp[i - 1][j - 1] + dp[i - j][j];
    cout << dp[n][k] << endl;
    system("pause");
    return 0;
}