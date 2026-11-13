#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int> > dp(m + 1, vector<int>(2));
    vector<int> w(n), v(n);
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];
    for (int i = 0; i < n; i++) {
        for (int j = m; j >= w[i]; j--) {
            dp[j][0] = max(dp[j][0], dp[j - w[i]][0] + v[i]);
            dp[j][1] = max(dp[j][1], dp[j - w[i]][1] + v[i]);
            if (j >= w[i] + k)
                dp[j][1] = max(dp[j][1], dp[j - w[i] - k][0] + 2 * v[i]);
        }
    }
    cout << max(dp[m][0], dp[m][1]) << endl;
    system("pause");
    return 0;
}