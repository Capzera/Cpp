#include <bits/stdc++.h>
using namespace std;
int n, m, k, x, a[101], dp[1 << 20];
int main() {
	cin >> n >> m >> k;
	memset(dp, 0x3f, sizeof(dp));
	dp[0] = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 1; j <= k; j++) {
			cin >> x;
			a[i] |= 1 << x - 1;
		}
	}
	for (int i = 0; i < 1 << m; i++)
		for (int j = 0; j < n; j++)
			dp[i | a[j]] = min(dp[i | a[j]], dp[i] + 1);
	if (dp[(1 << m) - 1] == 0x3f3f3f3f) cout << -1;
	else cout << dp[(1 << m) - 1];
	return 0;
}
