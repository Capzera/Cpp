#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int d, t, m, mod = 1e9 + 7;
    cin >> d >> t >> m;
    vector<int> dp(m + 1);
    dp[0] = 1;
    for (int i = 1; i <= t; i++)
        for (int j = m; j >= 0; j--) {
            if (d + i - 2 * j > 0) 
                dp[j] = (dp[j] + dp[j - 1]) % mod;
            else dp[j] = 0;
        }
    cout << dp[m] << endl;
    system("pause");
    return 0;
}