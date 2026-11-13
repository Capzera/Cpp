#include <bits/stdc++.h>
using namespace std;
long long n, x, y, sum, a[200010], ans = LLONG_MIN;
int main() {
    cin >> n >> x >> y;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        sum += a[i];
    }
    sort(a, a + n);
    for (int i = 1; i <= y; i++) sum -= 2 * a[n - i];
    for (int i = 0, j = n - 1; i <= x && j >= y; i++, j--) {
        ans = max(ans, sum);
        sum += a[j] - 2 * a[j - y];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}