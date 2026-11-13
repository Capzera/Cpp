#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
long long n, a[N], s[N], dp[N];
int q[N];
int check(long long x) {
	int head = 0, tail = 0;
	for (int i = 1; i <= n + 1; i++) {
		while (head <= tail && s[i - 1] - s[q[head]] > x) head++;
		dp[i] = a[i] + dp[q[head]];
		if (i <= n) {
			while (head <= tail && dp[q[tail]] >= dp[i]) tail--;
			q[++tail] = i;
		}
		/*
		for (int j = 0; j < i; j++) {
			if (s[i - 1] - s[j] > x) continue;
			dp[i] = min(dp[i], a[i] + dp[j]);
		} 
		*/
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
	long long l = 0, r = s[n] + 1, ans = 2e9;
	while (l < r) {
		long long mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = r = mid;
		} else l = mid + 1;
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
//https://codeforces.com/problemset/problem/1918/D
/*
二分答案一个x表示每个段的和都不能超过x
设dp[i] 表示第 i 个位置被阻断，且前 i - 1 个位置中所有未阻断的和都 小于等于x时的最小阻断和
dp[i] = a[i] + min(dp[j])  j ∈ [0, i) && s[i - 1] - s[j] <= x;




*/
