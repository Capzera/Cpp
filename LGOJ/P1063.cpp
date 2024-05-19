#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, ans = 0;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    nums.insert(nums.end(), nums.begin(), nums.end());
    vector<vector<int> > dp(2 * n, vector<int>(2 * n));
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i < 2 * n - len; i++) {
            int j = i + len - 1;
            for (int k = i; k < j; k++)
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j] + nums[i] * nums[k + 1] * nums[j + 1]);
        }
    }
    for (int i = 0; i < n; i++) 
        ans = max(ans, dp[i][i + n - 1]);
    cout << ans << endl;
    system("pause");
    return 0;
}