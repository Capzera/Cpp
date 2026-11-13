#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f3f3f3f3f
const int N = 1e5 + 5, M = 51;
int n, c[N], h[N];
long long x, dp[N];
void solve() {
	cin >> n >> x;
	long long sum = 0;
	for (int i = 1; i <= n; i++) {
		cin >> c[i] >> h[i];
		sum += h[i];
	}
	memset(dp, 0x3f, sizeof dp);
	dp[0] = 0;
	for (int i = 0; i <= n; i++) {
		for (int j = sum; j >= h[i]; j--) {
			if (dp[j - h[i]] + c[i] > x * (i - 1)) continue;
			dp[j] = min(dp[j], dp[j - h[i]] + c[i]);
		}
	}
	for (int i = sum; i >= 0; i--) {
		if (dp[i] == inf) continue;
		cout << i << '\n';
		return;
	}
	cout << "0\n";
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	int T;
	cin >> T;
	while (T--) solve();
	return 0;
}
//https://codeforces.com/problemset/problem/1974/E
/*
设 dp[j] 表示花费 j 元能获得的最大幸福度
x <= 1e8, 总钱最大值为 50 * 1e8 = 5e9, 超时 + 爆空间

考虑设 dp[j] 表示获得 j 幸福度最小需要花费 dp[j] 元

当前幸福度为j, 当前在第i个月，当前这个月花费c[i] 元可以获得 h[i] 幸福度
所以上一状态幸福度为 dp[j - h[i]] 
考虑边界为 dp = inf, dp[0] = 0
答案为 最大不为 inf 的dp[j] 的j的值

在第i个月，最大可支配钱 = x * (i - 1)
所以对第i个月， dp 必须满足 dp[j - h[i] + c[i]] <= (i - 1) * x;
*/
