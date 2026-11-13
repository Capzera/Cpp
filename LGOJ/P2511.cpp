#include <bits/stdc++.h>
using namespace std;
const int N = 5e4 + 10, p = 10007;
int n, m, loc[N], a[N];
int dp[N], sum[N];
bool check(int x) {
	int sum = 0, cnt = 0;
	for (int i = 1; i <= n; i++) {
		if (sum + a[i] > x) {
			cnt++;
			sum = 0;
		}
		if (cnt > m) return 0;
		sum += a[i];
	}
	return 1;
}
int main() {
	cin >> n >> m;
	int l = 0, r = 5e7 + 1, ans = 0;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		l = max(l, a[i]);
	}
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = r = mid;
		} else l = mid + 1;
	}
	cout << ans << " ";
	for (int i = 1, j = 1; i <= n; i++) {
		a[i] += a[i - 1];
		while (a[i] - a[j] > ans) j++;
		loc[i] = j; // 距离当前i最远的能使得这一段[j,i]的和小于等于ans位置
		if (a[i] <= ans) dp[i] = 1 ;
		sum[i] = (sum[i - 1] + dp[i]) % p;
	}
	ans = 0;
	for (int j = 1; j <= m; j++) { // 枚举组
		for (int i = 1; i <= n; i++) { // 枚举终点
			dp[i] = sum[i - 1];
			if (loc[i]) dp[i] = ((dp[i] - sum[loc[i] - 1]) % p + p) % p;
		}
		for (int i = 1; i <= n; i++) sum[i] = (sum[i - 1] + dp[i]) % p;
		ans = (ans + dp[n]) % p;
	}
	cout << ans;
	return 0;
}
/*
别家立直的时候用AI防守！！！







设dp[i][j] 前i根棍分成j组的方案总数
dp[i][1] sum[i] > ans
sum(dp[n][j]) % p; 


枚举分割起点为k

dp[k][j - 1]
dp[i][j]
sum[k + 1, i] <= ans 

dp[i][j] += dp[k][j - 1] (k ∈ [loc[i], i))
loc[i] 距离当前i点最远的位置j使得
sum[j, i] <= ans;
a[i] - a[j - 1] <= ans;
a[i] = sum[1..i]
1 4 2 3 3 5 7
ans = 8
0 1 5 7 10 13 18 25
loc[i] 




dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + c[i])
*/
