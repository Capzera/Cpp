#include <bits/stdc++.h>

using namespace std;

int main() {
    int n, W, V;
    cin >> n >> W >> V;
    int w, m, v;
    vector<vector<int> > dp(W + 1, vector<int> (V + 1));
    for (int i = 0; i < n; i++) {
        cin >> w >> m >> v;
        for (int j = W; j >= w; j--) {
            for (int k = V; k >= m; k--) {
                dp[j][k] = max(dp[j][k], dp[j - w][k - m] + v[i]);
            }
        }
    }
    cout << dp[W][V] << endl;
    system("pause");
    return 0;
}
/*
T : O(nWV)
M : O(WV)
*/