#include <bits/stdc++.h>
using namespace std;
const int N = 50, M = 2e6 + 10;
int n, m, mn = INT_MAX, ans, dp[M], w[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> w[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 2 * m; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + w[i]);
		}
	}
	for (int i = 1; i <= 2 * m; i++) {
		if (abs(dp[i] - m) < mn) {
			mn = abs(dp[i] - m);
			ans = i;
		}
	}
	cout << ans;
	return 0;
}
