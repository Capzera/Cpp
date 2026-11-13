#include <bits/stdc++.h>
using namespace std;
long long n, m, l, r, mx, a[4000001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> l >> r;
		a[l + r]++;
		mx = max(mx, l + r);
	}
	for (int i = 1; i <= mx << 1; i++) a[i] += a[i - 1];
	while (m--) {
		cin >> l >> r;
		cout << a[r << 1] - a[(l << 1) - 1] << endl;
	}
	return 0;
}
