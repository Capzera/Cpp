#include <bits/stdc++.h>

using namespace std;

int main(){
    int n,W;
    cin >> W >> n;
    vector<int> w(n), v(n);
    vector<int> dp(W + 1);
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];
    for (int i = 0; i < n; i++) {
        for (int j = w[i]; j <= W; j++)
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
    cout<<"MAX = "<<dp[W];
    system("pause");
    return 0;
}