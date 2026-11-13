#include <bits/stdc++.h>
using namespace std;
int t, n, m, d, ans, a[250001];
int main() {
	cin >> n >> m >> d;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			int idx = (i - 1) * m + (i & 1 ? j : m - j + 1);
			cin >> a[idx];
		}
	}
	for (int i = 1; i <= n * m; i++) a[i] += a[i - 1];
	for (int x = 1; x <= n * m; x += d) {
		if (x > 1 && !(a[x] - a[x - 1])) break;
		ans += a[min(x + d - 1, n * m)] - a[x - 1];
	}
	cout << ans;
	return 0;
}
