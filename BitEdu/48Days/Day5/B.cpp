#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int n, dp, ans = INT_MIN, last = INT_MIN;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> dp;
        dp += last > 0 ? last : 0//dp[i] = max(dp[i], dp[i - 1] + nums[i]);
        ans = max(ans, dp);
        last = dp;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
vector<int> nums(n);
for (int i = 0; i < n; i++) {
    cin >> nums[i];
}
int dp = nums[0];
for (int i = 1; i < n; i++) {
    dp = max(dp, nums[i] + dp);
    ans = max(ans, dp);
}
*/