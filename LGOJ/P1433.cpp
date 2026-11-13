#include <bits/stdc++.h>
using namespace std;
long long n;
double x[16], y[16], dis[16][16], dp[1 << 16][16], ans = DBL_MAX;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> x[i] >> y[i]; 
	for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++)
			dis[i][j] = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
	memset(dp, 0x7f, sizeof(dp));
	dp[1][0] = dp[0][0] = 0;
	for (int i = 3; i < 1 << n + 1; i += 2) {
		for (int j = 1; j <= n; j++) {
			if (i >> j & 1 ^ 1) continue;
			for (int k = 0; k <= n; k++) {
				if (j == k || i >> k & 1 ^ 1) continue;
				dp[i][j] = min(dp[i][j], dp[i ^ 1 << j][k] + dis[k][j]);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		ans = min(ans, dp[(1 << n + 1) - 1][i]);
	}
	printf("%.2lf", ans);
	return 0;
}
