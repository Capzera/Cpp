#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, dp = 1, dp0 = 0;
    cin >> n;
    while (n--) {
        long long dpt = dp;
        dp += dp0;
        dp0 = dpt;
    }
    cout << dp << endl;
    system("pause");
    return 0;
}