#include <bits/stdc++.h>
using namespace std;
const int N = 20, M = 200;
int n, m, k, r, ans, dp[M], w[N], c[N], a[N];
int main() {
	cin >> n >> m >> k >> r;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= m; i++) cin >> w[i];
	for (int i = 1; i <= m; i++) cin >> c[i];
	for (int i = 1; i <= n; i++) {
		for (int j = r; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + c[i]);
		}
	}
	int rest = -1;
	for (int i = 1; i <= r; i++) {
		if (dp[i] >= k) {
			rest = r - i;
			break;
		}
	}
	for (int i = 1; i <= n; i++) {
		if (rest < a[i]) break;
		rest -= a[i];
		ans++;
	}
	cout << ans;
	return 0;
}
