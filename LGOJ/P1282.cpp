#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, N = 5000, ans = n, a, b;
    cin >> n;
    vector<int> dp(10006, n); dp[N] = 0;
    for (int i = 0; i < n; i++) {
        cin >> a >> b;
        int dis = a - b;
        vector<int> dpt(dp);
        for (int j = 0; j <= 2 * N; j++) {
            dp[j] = min(dpt[j - dis], dpt[j + dis] + 1);
        }
    }
    for (int i = 0; i <= N; i++) {
        ans = min(dp[N + i], dp[N - i]);
        if (ans < n) {
            cout << ans << endl;
            break;
        }
    }
    system("pause");
    return 0;
}