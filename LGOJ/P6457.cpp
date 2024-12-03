#include <bits/stdc++.h>
using namespace std;
int n, m, x, ans, dp[202][202];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		dp[i][i] = dp[i + n][i + n] = x & 1;
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i <= 2 * n - len + 1; i++) {
			int j = i + len - 1;
			dp[i][j] = max(dp[i][i] - dp[i + 1][j], dp[j][j] - dp[i][j - 1]);
		}
	}
	for (int i = 1; i <= n; i++) {
		ans += dp[i][i] - dp[i + 1][i + n - 1] > 0;
	}
	cout << ans;
	return 0;
}
