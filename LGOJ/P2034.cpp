#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int n, k;
long long ans = LLONG_MAX, a[N], s[N], dp[N];
int head = 0, tail = 0, q[N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		s[i] = s[i - 1] + a[i];
	}
	for (int i = 1; i <= n + 1; i++) {
		while (head <= tail && q[head] < i - k - 1) head++;
		dp[i] = max(dp[i], dp[q[head]] + s[i - 1] - s[q[head]]);
		while (head <= tail && dp[q[tail]] - dp[i] <= s[q[tail]] - s[i]) tail--;
		q[++tail] = i;
	}
	cout << dp[n + 1];
	return 0;
}
