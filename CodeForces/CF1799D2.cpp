#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 5;
long long n, k, a[N], hot[N], cold[N], dp[N];
void solve() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int j = 1; j <= k; j++) cin >> cold[j];
	for (int j = 1; j <= k; j++) cin >> hot[j];
	for (int i = 1; i <= k; i++) dp[i] = 1e18;
	long long s = 0, mn = 0;
	for (int i = 1; i <= n; i++) {
		int lst = a[i - 1], x = a[i];
		if (x == lst) {
			s += hot[x];
		} else {
			s += cold[x];
			dp[lst] = min(mn, dp[x] + hot[x] - cold[x]);
			mn = min(mn, dp[lst]);
		}
	}
	cout << mn + s << '\n';
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T = 1;
	cin >> T;
	while (T--) solve();
	return 0;
}
//https://codeforces.com/problemset/problem/1799/D2
/*
设 dp[i][j] 表示处理完前i个程序, CPU1 最后一次运行i, CPU2 最后一次运行j的最小时间 O(nk^2)
x = a[k];
1. dp[x][j] = min(dp[x][j], dp[i][j] + (x == i ? hot[x] : cold[x]);
2. dp[i][x] = min(dp[i][x], dp[i][j] + (x == j ? hot[x] : cold[x]);



新的储存值 = 新的实际代价 - 新的全局偏移量

真正的 dp[j] = 假的 dp[j] + 偏移量(s)
1) 当 x == lst 时
偏移量 s += hot[x];
2) 当 x != lst 时
偏移量 s += cold[x];
s_new = s_old + cold[x];

尝试更换活跃CPU，也就是说，令 lst 为活跃CPU，对于x现在使用待机CPU

dp[lst] = min(mn + s_old + cold[x], dp[x] + s_old + hot[x]) - s_new;
dp[lst] = min(mn, dp[x] + hot[x] - cold[x]);
*/
