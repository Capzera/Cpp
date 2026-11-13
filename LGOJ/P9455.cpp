#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, a[N], b[N];
bool check(long long x) {
	long long loc = a[1];
	for (int i = 1; i <= n; i++) {
		if (a[i] > loc) return 0;
		loc = max(a[i] + b[i] + x, loc);
	}
	return 1;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> b[i];
	}
	long long l = 0, r = 2e9, ans = 0;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = mid;
			r = mid;
		} else l = mid + 1;
	}
	cout << ans;
	return 0;
}
