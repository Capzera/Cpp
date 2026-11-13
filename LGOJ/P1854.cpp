#include <bits/stdc++.h>
using namespace std;
const int N = 110;
int n, m, ans = INT_MIN, dp[N][N], a[N][N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			dp[i][j] = INT_MIN;
		}
	}
	for (int j = 1; j <= m; j++) dp[0][j] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= m; j++) {
			for (int k = i - 1; k < j; k++) {
				dp[i][j] = max(dp[i][j], dp[i - 1][k] + a[i][j]);
			}
			if (i == n) ans = max(ans, dp[i][j]);
		}
	}
	cout << ans << '\n';
	vector<int> v;
	for (int i = n; i; i--) {
		for (int j = i; j <= m; j++) {
			if (dp[i][j] == ans) {
				ans -= a[i][j];
				v.push_back(j);
				break;
			}
		}
	}
	for (int i = v.size() - 1; ~i; i--) cout << v[i] << " ";
	return 0;
}
/*
	dp[i][j] 表示前i个花瓶中插j朵花的最大价值
	dp[i][j] = max(dp[i][j], dp[i - 1][k] + a[i][j])
	
*/
