#include <bits/stdc++.h>
using namespace std;
int n, x, j, ans, dp[10001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x >> x;
		int mx = 0;
		while (cin >> j, j) {
			mx = max(mx, dp[j]);
		}
		dp[i] = x + mx;
		ans = max(ans, dp[i]);
	}
	cout << ans;
	return 0;
}
