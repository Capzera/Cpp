#include <bits/stdc++.h>

using namespace std;

int main(){
    int n,W;
    cin >> W >> n;
    vector<int> w(n), v(n);
    vector<int> dp(W + 1);
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];
    for (int i = 0; i < n; i++) {
        for (int j = W; j >= w[i]; j--)
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
    cout<<"MAX = "<<dp[W];
    system("pause");
    return 0;
}
/*

    dp(n, vector<int> (W + 1))
    for (int i = 0; i < n; i++){
        for (int j = W; j >= 0; j--) {
            if (i) dp[i][j] = dp[i - 1][j];
            if (j >= v[i])
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + v[i]);
        }
    }
    cout<<dp[n - 1][W];
*/