#include <bits/stdc++.h>
using namespace std;
const int N = 505, K = 301;
int t, n, m, x, ans, dp[N][N][K];
char ch;
void solve() {
	cin >> n >> m >> x;
	int ans = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> ch;
			for (int k = 0; k <= x; k++) {
				dp[i][j][k] = max(dp[i - 1][j][k], dp[i][j - 1][k]) + (ch == '1');
				if (ch == '?' && k) dp[i][j][k] = max(dp[i - 1][j][k - 1], dp[i][j - 1][k - 1]) + 1;
				if (i == n && j == m) ans = max(ans, dp[i][j][k]);
			}
		}
	}
	cout << ans << '\n';
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) solve();
	return 0;
}
