#include <bits/stdc++.h>
using namespace std;
int main() {
    const int n = 30;
    long long dp2 = 1, dp1 = 2, dp;
    for (int i = 2; i <= n; i++) {
        dp = dp2 + dp1;
        dp2 = dp1;
        dp1 = dp;
    }
    cout << dp << endl;
    system("pause");
    return 0;
}
/*
C29 0 + C28
*/