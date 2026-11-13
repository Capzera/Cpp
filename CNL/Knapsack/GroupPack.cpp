#include <bits/stdc++.h>

using namespace std;

int main(){
    int W, G;
    cin >> G >> W;
    vector<int> dp(W + 1);
    for (int i = 0; i < G; i++) {
        int n;//当前组里有几个元素
        cin >> n;
        vector<int> w(n + 1) , v(n + 1);
        for (int j = 0; j < n; j++) 
            cin >> w[j] >> v[j];
        for (int j = W; j >= 0; j--) {
            for (int k = 0; k < n; k++) {
                if (j >= w[k])
                    dp[j] = max(dp[j], dp[j - w[k]] + v[k]);
            }
        }
    }
    cout << "MAX = " <<dp[W];
    system("pause");
    return 0;
}
/*
3 5
2
1 2
2 4
1
3 4
1
4 5





*/