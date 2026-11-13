#include <bits/stdc++.h>
using namespace std;
long long n, w, x, a[100010], ans = LLONG_MAX;
long long get(long long x) {
	long long cost = x * w;
	for (int i = 1; i <= n; i++) {
		cost += max(a[i] - x - i + 1, 0ll);
	}
	return cost;
}
int main() {
	cin >> n >> w;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	sort(a + 1, a + n + 1);
	long long l = 0, r = a[n] + n - 1;
	while (r - l > 3) {
		long long m1 = l + (r - l) / 3;
		long long m2 = r - (r - l) / 3;
		long long f1 = get(m1);
		long long f2 = get(m2);
		if (f1 < f2) {
			r = m2;
		} else l = m1;
	}
	for (int x = l; x <= r; x++) {
		ans = min(ans, get(x));
	}
	cout << ans;
	return 0;
}
/*


3 2 2 1


3 4 5 5
3 3 3 2
*/
