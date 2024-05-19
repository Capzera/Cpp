#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, m, s, ans = LLONG_MAX;
    cin >> n >> m >> s;
    vector<int> w(n), v(n), left(m), right(m);
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];
    for (int i = 0; i < m; i++) cin >> left[i] >> right[i];
    long long l = 0, r = 1e6 + 10;
    while (l < r) {
        long long W = l + (r - l) / 2, Y = -s;
        vector<long long> sumw(n + 1), sumv(n + 1);
        for (int i = 0; i < n; i++) {
            sumw[i + 1] = sumw[i], sumv[i + 1] = sumv[i];
            if (w[i] >= W) sumw[i + 1]++, sumv[i + 1] += v[i];
        }
        for (int i = 0; i < m; i++) {
            int x = left[i], y = right[i];
            Y += 1ll * (sumw[y] - sumw[x - 1]) * (sumv[y] - sumv[x - 1]);
        }
        if (Y > 0) l = W + 1;
        else r = W;
        ans = min(ans, abs(Y));
    }
    cout << ans << endl;
    system("pause");
    return 0;
}