#include <bits/stdc++.h>
using namespace std;
int n, d;
double ans = 1e9, a[20][20], x[20], y[20], dp[1 << 20][20];
int main() {
	cin >> n >> d;
	for (int i = 0; i < n; i++) cin >> x[i] >> y[i];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double dis = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
			if (dis <= d) a[i][j] = dis;
			else a[i][j] = 1e9;
		}
	}
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				a[i][j] = min(a[i][j], a[i][k] + a[k][j]);
	memset(dp, 0x7f, sizeof(dp)), dp[1][0] = 0;
	for (int i = 1; i < 1 << n; i++) {
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			for (int k = 0; k < n; k++) {
				if (((i ^ 1 << j) >> k & 1) == 0) continue;
				dp[i][j] = min(dp[i][j], dp[i ^ 1 << j][k] + a[k][j]);
			}
		}
	}
	for (int i = 0; i < n; i++) ans = min(ans, dp[(1 << n) - 1][i] + a[i][0]);
	printf("%.2lf", ans);
	return 0;
}
