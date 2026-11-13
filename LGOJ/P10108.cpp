#include <bits/stdc++.h>
using namespace std;
long long n, m, ans = INT_MIN, a[101], b[10001], dp[10001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	sort(a + 1, a + m + 1);
	dp[1] = b[1];
	for (int i = 2; i <= n; i++) {
		dp[i] = INT_MIN;
		for (int j = 1; j <= m; j++) {
			if (a[j] >= i) break;
			dp[i] = max(dp[i], dp[i - a[j]] + b[i]);
		}
		if (i > n - a[m]) ans = max(ans, dp[i]);
	}
	cout << ans;
	return 0;
}
