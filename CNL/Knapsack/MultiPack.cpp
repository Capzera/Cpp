#include <bits/stdc++.h>

using namespace std;

int main(){
    int n,W;
    cin >> W >> n;
    vector<int> w(n), v(n), s(n);
    vector<int> dp(W + 1);
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i] >> s[i];
    for (int i = 0; i < n; i++) {
        for (int j = W; j >= w[i]; j--) {
            for (int k = 1; k <= s[i] && k * w[i] <= j; k++)
                dp[j] = max(dp[j], dp[j - w[i] * k] + v[i] * k);
        }
    }
    cout<<"MAX = "<< dp[W] <<endl;
    system("pause");
    return 0;
}