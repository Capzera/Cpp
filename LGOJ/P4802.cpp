#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, w, dp[1 << 18][18], mp[19][19], ans = 0x8f8f8f8f;
int main() {
	cin >> n >> m;
	memset(dp, 0x8f, sizeof(dp));
	dp[1][0] = 0; // 设最后一位为位置0
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		mp[u][v] = w;
	}
	for (int i = 3; i < 1 << n; i += 2) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			for (int k = 0; k < n; k++) {
				if (j == k || i >> k & 1 ^ 1 || mp[k][j] == 0) continue;
				dp[i][j] = max(dp[i][j], dp[i ^ (1 << j)][k] + mp[k][j]);
			}
		}
	}
	for (int i = (1 << (n - 1)) + 1; i <= (1 << n) - 1; i += 2) {
		ans = max(ans, dp[i][n - 1]);
	}
	cout << ans;
	return 0;
}
