#include <bits/stdc++.h>
using namespace std;
const int N = 110, M = 1e5 + 10;
int n, m, ans, a[N], dp[2][M];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		m += a[i];
	}
	dp[0][0] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = m; j >= 0; j--) {
			dp[i & 1][j] |= dp[i & 1 ^ 1][j];
			dp[i & 1][j] |= dp[i & 1 ^ 1][abs(j - a[i])];
			if (j + a[i] <= m) dp[i & 1][j] |= dp[i & 1 ^ 1][j + a[i]];
		}
	}
	for (int j = 1; j <= m; j++) ans += dp[n & 1][j];
	cout << ans;
	return 0;
}
设dp[i][j] 使用表示前i个砝码，重量为j的是否可以称出
if (j + a[i] <= m) dp[i][j] |= dp[i - 1][j + a[i]];
dp[i][j] |= dp[i][abs(j - a[i])]


dp[i][j] --> dp[i & 1][j]
dp[i - 1][] ---> dp[i & 1 ^ 1][]

dp[j] 花费为j时能获得最大武器强度

