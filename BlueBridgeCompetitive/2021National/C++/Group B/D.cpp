#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<long long> dp(2022, LLONG_MAX);
    dp[0] = 0;
    for (int i = 1; i <= 2021; i++)
        for (int j = 0; j < i; j++)
            dp[i] = min(dp[i], 1 + 1LL * 2 * dp[j] + 3 * dp[i - j - 1] + 1LL * j * j * (i - j - 1));
    cout << dp[2021] << endl;
    system("pause");
    return 0;
}