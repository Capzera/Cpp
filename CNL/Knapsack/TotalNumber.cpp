#include <bits/stdc++.h>

using namespace std;

int main() {
    int n,W;
    cin >> n >> W;
    vector<int> w(n), v(n), dp(W + 1), g(W + 1);
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> v[i];
    }
    g[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = W; j >= w[i]; j--) {
            int maxv = max(dp[j], dp[j - w[i]] + v[i]);
            int sum = 0;
            if (dp[j] == maxv) sum += g[j];
            if (dp[j - w[i]] + v[i] == maxv) sum += g[j - w[i]];
            dp[j] = maxv;
            g[j] = sum;
        }
    }
    cout << "Total Number of the Program£º" << g[W] << endl;
    system("pause");
    return 0;
}