#include <bits/stdc++.h>
using namespace std;
const int N = 70, M = 32010;
int n, m, w[N], c[N], p[N][3], q[N], dp[M];
int main() {
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		cin >> w[i] >> c[i] >> q[i];
		c[i] *= w[i];
		if (q[i]) p[q[i]][++p[q[i]][0]] = i;
	}
	for (int i = 1; i <= n; i++) {
		if (q[i]) continue;
		for (int j = m; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
			int f1 = p[i][1], f2 = p[i][2];
			if (f1 && j - w[i] - w[f1] >= 0) dp[j] = max(dp[j], dp[j - w[i] - w[f1]] + c[i] + c[f1]);
			if (f2 && j - w[i] - w[f2] >= 0) dp[j] = max(dp[j], dp[j - w[i] - w[f2]] + c[i] + c[f2]);
			if (f1 && f2 && j - w[i] - w[f1] - w[f2] >= 0) dp[j] = max(dp[j], dp[j - w[i] - w[f1] - w[f2]] + c[i] + c[f1] + c[f2]);
		}
	}
	cout << dp[m];
	return 0;
}
