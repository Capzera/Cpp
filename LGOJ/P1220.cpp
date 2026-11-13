#include <bits/stdc++.h>
using namespace std;
const int N = 60;
long long n, c, a[N], w[N], sum[N], dp[N][N][2];

int main() {
	cin >> n >> c;
	memset(dp, 0x3f, sizeof(dp));
	dp[c][c][0] = dp[c][c][1] = 0;
	for (int i = 1; i <= n; i++) {
		cin >> a[i] >> w[i];
		sum[i] = sum[i - 1] + w[i];
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i + len - 1 <= n; i++) {
			int j = i + len - 1;
			long long f1 = sum[n] - (sum[j] - sum[i]);
			long long f2 = sum[n] - (sum[j - 1] - sum[i - 1]);
			dp[i][j][0] = min(dp[i + 1][j][0] + (a[i + 1] - a[i]) * f1, dp[i + 1][j][1] + (a[j] - a[i]) * f1);
			dp[i][j][1] = min(dp[i][j - 1][1] + (a[j] - a[j - 1]) * f2, dp[i][j - 1][0] + (a[j] - a[i]) * f2);
		}
	}
	cout << min(dp[1][n][0], dp[1][n][1]);
	return 0;
}
