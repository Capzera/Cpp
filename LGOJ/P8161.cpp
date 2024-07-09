#include <bits/stdc++.h>
using namespace std;
long long n, m, a[300010], b[300010];
bool check(long long x) {
    __int128 sum = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] * m >= x) {
			sum += (a[i] * m - x) / a[i];
		} else {
			sum -= (x - a[i] * m + b[i] - 1) / b[i];
		}
	}
	return sum >= 0;
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (int i = 0; i < n; i++) {
		cin >> b[i];
		a[i] = max(a[i], b[i]);
	}
	long long l = 0, r = LLONG_MAX;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) l = mid + 1;
		else r = mid;
	}
	cout << l - 1;
	return 0;
}
