#include <bits/stdc++.h>
using namespace std;
int n, m, d, s, t, z, dp[101];
int main() {
	memset(dp, 0x3f, sizeof(dp));
	cin >> n >> m >> d >> s;
	dp[0] = 0;
	for (int i = 1; i <= m; i++) {
		cin >> t >> z;
		for (int j = n; j >= 0; j--) {
			for (int k = 0; k <= min(j, z); k++) {
				dp[j] = min(dp[j], dp[j - k] + k * t + d);
			}
		}
	}
	if (dp[n] > 10000) cout << "impossible";
	else cout << dp[n];
	return 0;
}
/*
f[j] 前i辆车走了j个学生的最小花费
f[i][j] = min(f[i][j], f[i - 1][j - k] + k * t[i] + d);
*/
