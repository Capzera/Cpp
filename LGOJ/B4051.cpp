#include <bits/stdc++.h>
using namespace std;
int n, m, x, mx = INT_MIN, a[100010];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> x, mx = max(mx, x);
	for (int i = 1; i <= m; i++) cin >> a[i];
	sort(a + 1, a + m + 1, greater<int>());
	for (int i = 1; i <= m; i++) {
		if (n > 1 && a[i] <= 0) break;
		mx += a[i];
	}
	cout << mx;
	return 0;
}
