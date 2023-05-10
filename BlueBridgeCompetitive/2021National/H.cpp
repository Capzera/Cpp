#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, k, ans = 0;
    cin >> n >> k;
    vector<vector<long long>> dp(60, vector<long long>(60));
    for (int i = 0; i < 60; i++) {
        dp[i][0] = 1;
        for (int j = 1; j <= i; j++)
            dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
    }
    vector<int> nums;
    while (n) {
        nums.push_back(n % 2);
        n /= 2;
    }
    for (int i = nums.size() - 1, last = 0; i >= 0; --i) {
        if (nums[i]) {
            ans += dp[i][k - last];
            if (++last > k) break;
        }
        if (!i && last == k) ans++;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, k, ans = 0;
    cin >> n >> k;
    vector<vector<long long>> dp(60, vector<long long>(60));
    for (int i = 0; i < 60; i++) {
        dp[i][0] = 1;
        for (int j = 1; j <= i; j++)
            dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
    }
    while (n >= 0 && k >= 0) {
        int i = log(n + 1) / log(2);
        ans += dp[i][k--];
        n -= 1LL << i;. 
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
*/