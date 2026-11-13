#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
long long n, l, r, ans = LLONG_MIN, head = 1, tail, a[N], dp[N], q[N];
int main() {
	cin >> n >> l >> r;
	memset(dp, 0xcf, sizeof(dp));
	dp[0] = 0;
	for (int i = 0; i <= n; i++) cin >> a[i];
	for (int i = l, j = 0; i <= n; i++, j++) {
		while (head <= tail && dp[q[tail]] <= dp[j]) tail--;
		q[++tail] = j;
		while (head <= tail && q[head] < i - r) head++;
		dp[i] = dp[q[head]] + a[i];
		if (i > n - r) ans = max(ans, dp[i]);
	}
	cout << ans;
	return 0;
}
