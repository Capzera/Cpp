#include <bits/stdc++.h>
using namespace std;
const int N = 200010;
long long n, m, s, sum, ans = LLONG_MAX, mx = LLONG_MIN, mn = LLONG_MAX;
long long w[N], v[N], lt[N], rt[N], sw[N], sv[N];
bool check(long long x) {
	sum = 0;
	for (int i = 1; i <= n; i++) {
		sw[i] = sw[i - 1] + (w[i] >= x);
		sv[i] = sv[i - 1] + v[i] * (w[i] >= x);
	}
	for (int i = 1; i <= m; i++) {
		sum += (sw[rt[i]] - sw[lt[i] - 1]) * (sv[rt[i]] - sv[lt[i] - 1]);
	}
	return sum > s;
}
int main() {
    cin >> n >> m >> s;
	for (int i = 1; i <= n; i++) {
		cin >> w[i] >> v[i];
		mn = min(mn, w[i]);
		mx = max(mx, w[i]);
	}
	for (int i = 1; i <= m; i++) {
		cin >> lt[i] >> rt[i];
	}
	long long l = mn, r = mx + 1;
	while (l < r) {
		long long mid = l + (r - l) / 2;
		if (check(mid)) l = mid + 1;
		else r = mid;
		ans = min(ans, abs(sum - s));
	}
    cout << ans << endl;
    return 0;
}
