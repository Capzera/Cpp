#include <bits/stdc++.h>
using namespace std;
const int N = 5e3 + 5;
long long n, k, a[N], hot[N], cold[N], dp[N][N];
void solve() {
	cin >> n >> k;
	long long ans = LLONG_MAX;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= k; j++) cin >> cold[j];
	for (int j = 1; j <= k; j++) cin >> hot[j];
	for (int i = 0; i <= n; i++) for (int j = 0; j <= k; j++) dp[i][j] = 1e18;
	dp[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		long long lst = a[i - 1], x = a[i], mn = 2e18;
		for (int j = 0; j <= k; j++) {
			if (dp[i - 1][j] >= 1e18) continue;
			dp[i][j] = dp[i - 1][j] + (x == lst ? hot[x] : cold[x]);
			if (x != lst) mn = min(mn, dp[i - 1][j] + (x == j ? hot[x] : cold[x]));
		}
		if (x != lst) dp[i][lst] = min(dp[i][lst], mn);	
	}
	for (int j = 0; j <= k; j++) ans = min(ans, dp[n][j]);
	cout << ans << '\n';
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
//https://codeforces.com/problemset/problem/1799/D1
/*
注意到处理完第 i 个程序后，a[i] 一定在某个CPU上，那个CPU最后运行的程序一定是a[i]；
所以我们只需要记录另一个CPU
设 dp[x][i] 表示处理完x 后CPU 1 是 x，CPU2 是i的最小代价
dp[i][j] = dp[j][i]
处理下一个元素a[k + 1] 时
x = a[k], y = a[k + 1]
放在CPU1 ： 新状态为 dp[y][i], 代价增加 x == y ? hot[y] : cold[y]
放在CPU2 ： 新状态为 dp[y][x], 代价增加 x == y ? hot[y] : cold[y];
dp[x][i] 可以转移到 dp[y][i], dp[y][x]
时间复杂度 O(nk)

如果 x == lst, 整个dp数组全部会加上 hot[x]

如果 x != lst, 整个dp数组全部会加上 cold[x]，除了 x != lst 且 j == x

新状态 dp[i][
*/
