#include <bits/stdc++.h>
using namespace std;
long long a[100010], n, v, q, x, y, ans;
int main() {
	cin >> n >> q >> v;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	while (q--) {
		cin >> x >> y;
		a[x] = y;
	}
	for (int i = 1; i <= n; i++) {
		ans += a[i] >= v;
	}
	cout << ans;
	return 0;
}
