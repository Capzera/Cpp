#include <bits/stdc++.h>
using namespace std;
int n, x, ans, dp[32];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		int mx = 0;
		for (int j = 0; j < 32; j++) {
			if (x >> j & 1) mx = max(mx, dp[j] + 1);
		}
		for (int j = 0; j < 32; j++) {
			if (x >> j & 1) dp[j] = mx;
		}
		ans = max(ans, mx);
	}
	cout << ans;
	return 0;
}
