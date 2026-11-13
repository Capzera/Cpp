#include <bits/stdc++.h>
using namespace std;
int main() {
    long long ans = LLONG_MIN, n, x, y;
    cin >> n >> x >> y;
    vector<int> a(n), sum(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + a[i];
    }
    for (int i = 0; i <= min(x, n); i++) {
        if (i + y > n) ans = max(ans, -1ll * sum[n - i]);
        else ans = max(ans, 2ll * sum[n - i - y] - sum[n - i]);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}