#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, f[N], a[N];
double k, c, w, dp[N];
int main() {
	cin >> n >> k >> c >> w;
	for (int i = 1; i <= n; i++) cin >> f[i] >> a[i];
	for (int i = n; i > 0; i--) {
		if (f[i] == 1) { // 第i搜为资源型
			dp[i] = max(dp[i + 1], dp[i + 1] * (1 - 0.01 * k) + a[i]);
		} else {
			dp[i] = max(dp[i + 1], dp[i + 1] * (1 + 0.01 * c) - a[i]);
		}
	}
	printf("%.2lf", dp[1] * w);
	return 0;
}
