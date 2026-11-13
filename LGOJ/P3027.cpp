#include <bits/stdc++.h>
using namespace std;
const int N = 1e2 + 10, M = 1e5 + 10;
long long n, m, ans, r[N], c[N], dp[M];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> c[i] >> r[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = c[i]; j <= m; j++) {
			dp[j] = max(dp[j], dp[j - c[i]] + r[i] - c[i]);
		}
	}
	for (int i = 1; i <= m; i++) ans = max(ans, dp[i] + m - i);
	cout << ans;
	return 0;
}
