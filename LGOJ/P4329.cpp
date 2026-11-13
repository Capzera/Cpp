#include <bits/stdc++.h>
using namespace std;
int n;
double dp[1 << 20], a[20][20];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> a[i][j], a[i][j] *= 0.01;
	dp[0] = 1;
	for (int i = 0; i < 1 << n; i ++) {
		int k = __builtin_popcount(i);
		for (int j = 0; j < n; j++) {
			if (i >> j & 1 ^ 1) continue;
			dp[i] = max(dp[i], dp[i ^ 1 << j] * a[k - 1][j]);
		}
	}
	printf("%.6lf", dp[(1 << n) - 1] * 100);
	return 0;
}
