#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
long long n, a[N], s[N], dp[N];
long long q[N];
int check(long long x) {
	int head = 0, tail = 0;
	for (int i = 1; i <= n + 1; i++) {
		while (head <= tail && s[i - 1] - s[q[head]] > x) head++;
		dp[i] = a[i] + dp[q[head]];
		if (i <= n) {
			while (head <= tail && dp[q[tail]] >= dp[i]) tail--;
			q[++tail] = i;
		}
	}
	return dp[n + 1] <= x;
}
void solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		s[i] = s[i - 1] + a[i];
	}
	a[n + 1] = 0;
	long long l = 0, r = s[n], ans = 2e9;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) r = ans = mid;
		else l = mid + 1;
	}
	cout << ans << '\n';
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}





