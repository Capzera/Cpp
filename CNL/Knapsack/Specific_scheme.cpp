#include <bits/stdc++.h>

using namespace std;

int main() {
    int n,W;
    cin >> n >> W;
    vector<int> w(n), v(n);
    vector<vector<int> > dp(n + 1, vector<int> (W + 1));
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> v[i];
    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j <= W; j++) {
            dp[i][j] = dp[i + 1][j];
            if (j >= w[i])
                dp[i][j] = max(dp[i][j], dp[i + 1][j - w[i]] + v[i]);
        }
    }
    for (int i = 0, j = W; i < n; i++) {
        if (j >= w[i] && dp[i][j] == dp[i + 1][j - w[i]] + v[i]) {
            cout << i << " ";
            j -= w[i];
        }
    }
    system("pause");
    return 0;
}