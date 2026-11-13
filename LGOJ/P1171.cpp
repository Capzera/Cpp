#include <bits/stdc++.h>
using namespace std;
int n, u, v, w, dp[1 << 20][20], mp[20][20], ans = 0x3f3f3f3f;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> mp[i][j];
	memset(dp, 0x3f, sizeof(dp));
	dp[1][0] = 0;
	for (int i = 1; i < 1 << n; i += 2) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			for (int k = 0; k < n; k++) {
				if (j == k || i >> k & 1 ^ 1) continue;
				dp[i][j] = min(dp[i][j], dp[i ^ (1 << j)][k] + mp[k][j]);
			}
		}
	}
	for (int j = 0; j < n; j++) {
		ans = min(ans, dp[(1 << n) - 1][j] + mp[j][0]);
	}
	cout << ans	;
	return 0;
}

/*
dp[mask][j] 表示状态为mask，当前点为j的最短路
dp[mask][j] = min(dp[mask ^ (1 << j)][k] + mp[k][j]);
*/
