#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 2019;
    vector<int> prime, st(n + 1);
    vector<long long> dp(n + 1);
    dp[0] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) {
            prime.emplace_back(i);
            for (int j = n; j >= i; --j)
                dp[j] += dp[j - i];
        }
        for (int j = 0; j < prime.size() && prime[j] * i <= n; j++)
            st[prime[j] * i] = 1;
    }
    cout << dp[n] << endl;
    system("pause");
    return 0;
}