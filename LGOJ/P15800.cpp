#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, ans, a[N], b[N], dp[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= n; j++) cin >> b[j], b[j] += j;
	for (int i = 1; i <= n; i++) {
		dp[i] = max(dp[i], dp[i - 1]);
		ans = max(ans, dp[i] + a[i]);
		if (b[i] <= n) {
			dp[b[i]] = max(dp[b[i]], dp[i] + a[i]);
		}
	}
	cout << ans;
	return 0;
}

