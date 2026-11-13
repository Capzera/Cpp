#include <bits/stdc++.h>
using namespace std;
long long n, x, y, a[200010], sum[200010], ans = LLONG_MIN;
int main() {
    cin >> n >> x >> y;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a, a + n);
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + a[i];
    }
    for (int i = 0; i <= x; i++) {
        ans = max(ans, 2 * sum[max(n - i - y, 0LL)] - sum[n - i]);
    }
    cout << ans << endl;
    return 0;
}