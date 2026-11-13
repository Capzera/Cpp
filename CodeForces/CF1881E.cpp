#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;
long long n, a[N], dp[N];
void solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i] = 0;
	}
	dp[n + 1] = 0;
	for (int i = n; i; i--) {
		dp[i] = dp[i + 1];
		if (i + a[i] <= n) dp[i] = max(dp[i], 1 + a[i] + dp[i + a[i] + 1]);
	}
	cout << n - dp[1] << '\n';
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
//https://codeforces.com/problemset/problem/1881/E
/*
设 dp[i] 表示从位置 i 开始到末尾，最多能保留 dp[i] 个元素
考虑第 i 个元素：
1. 如果删除第 i 个元素 dp[i] = dp[i + 1]
2. 如果作为新的开始 dp[i] = 1 + a[i] + dp[i + a[i] + 1]

dp[i] = max(dp[i + 1], 1 + a[i] + dp[i + a[i] + 1]);

边界： dp[n + 1] = 0;
答案： n - dp[1];

*/
