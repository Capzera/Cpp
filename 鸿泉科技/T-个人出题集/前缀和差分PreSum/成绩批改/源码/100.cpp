#include <bits/stdc++.h>
using namespace std;
long long d[5000010], a[5000010];
int main() {
	memset(d, 0, sizeof(d));
	memset(a, 0, sizeof(a));
	long long n, p, x, y, z, mx = LLONG_MIN, mn = LLONG_MAX;
	cin >> n >> p;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		d[i] = a[i] - a[i - 1];
	}
	for (int i = 0; i < p; i++) {
		cin >> x >> y >> z;
		d[x] += z;
		d[y + 1] -= z;
	}
	for (int i = 1; i <= n; i++) {
		a[i] = a[i - 1] + d[i];
		mx = max(mx, a[i]);
		mn = min(mn, a[i]);
	}
	cout << mx << " " << mn;
	return 0;
}
