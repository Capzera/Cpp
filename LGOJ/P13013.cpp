#include <bits/stdc++.h>
using namespace std;
long long n, m, a, b, ans;
bool check(long long s) {
	if (a == b) return a * s <= min(n, m);
	long long L = ceil((1.0 * a * s - m) / (a - b));
	long long R = floor((1.0 * n - b * s) / (a - b));
	return L <= s && R >= 0 && L <= R;
}
int main() {
	cin >> n >> m >> a >> b;
	if (n < m) swap(n, m);
	if (a < b) swap(a, b);
	long long l = 0, r = max(n, m) / min(a, b) + 1;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			l = mid + 1;
		} else r = mid;
	}
	cout << ans;
	return 0;
}

