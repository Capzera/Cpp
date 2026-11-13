#include <bits/stdc++.h>
using namespace std;
long long n, k, m, x, ans = INT_MAX, a[100001];
int main() {
	cin >> n >> k >> m;
	while (m--) cin >> x, a[x] = 1;
	for (int i = 1; i <= n; i++) {
		a[i] += a[i - 1];
		if (i >= k) ans = min(ans, a[i] - a[i - k]);
	}
	cout << ans;
	return 0;
}
