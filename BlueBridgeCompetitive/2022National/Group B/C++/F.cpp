#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, k, ans = 0;
    cin >> n >> m >> k;
    int moon[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    vector<int> dp(366);
    for (int i = 0, x, y, z; i < n; i++) {
        cin >> x >> y >> z;
        int day = moon[x - 1] + y;
        dp[day] = max(dp[day], z);
    }
    for (int i = 1; i <= 365; i++) {
        dp[i] = max(dp[i] + dp[max(i - k, 0)], dp[i - 1]);
        if (dp[i] > m) dp[i] = dp[i - 1];
    }
    cout << dp[365] << endl;
    system("pause");
    return 0;
}
/*
7 14 3
1 1 1
1 2 2
1 3 4
1 4 5
1 6 8
1 1 10
1 3 6
*/