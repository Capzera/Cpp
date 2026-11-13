#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
long long n, k, d, a[N], sum[N], dp[N];
int main() {
	cin >> n >> k >> d;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1, greater<long long>());
	for (int i = 1; i <= n; i++) sum[i] = sum[i - 1] + a[i];
	for (int i = 1; i <= n; i++) {
		dp[i] = dp[i - 1] + max(0ll, a[i] - d);
		if (i >= k) dp[i] = min(dp[i], dp[i - k] + sum[i - 1] - sum[i - k]);
	}
	cout << dp[n];
	return 0;
}
