#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, dp = INT_MIN, ans = INT_MIN, put;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> put;
        dp = put + max(dp, 0);
        ans = max(ans, dp);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}