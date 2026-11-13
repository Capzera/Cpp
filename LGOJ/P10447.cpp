#include <bits/stdc++.h>
using namespace std;
int n, a[20][20], dp[1 << 20][20], ans = 0x3f3f3f3f;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> a[i][j];
	memset(dp, 0x3f, sizeof(dp));
	dp[1][0] = 0;
	for (int i = 0; i < 1 << 20; i++) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			for (int k = 0; k < n; k++) {
				if (j == k || i >> k & 1 ^ 1) continue;
				dp[i][j] = min(dp[i][j], dp[i ^ (1 << j)][k] + a[k][j]);
			}
		}
	}
	cout << dp[(1 << n) - 1][n - 1];
	return 0;
}
