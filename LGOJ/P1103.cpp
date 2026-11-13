#include <bits/stdc++.h>
using namespace std;
int n, k, ans = INT_MAX;
int main() {
	cin >> n >> k;
	k = n - k;
	vector<vector<int> > a(n + 1, vector<int>(2)), dp(n + 1, vector<int> (k + 1, INT_MAX));
	for (int i = 1; i <= n; i++) {
		cin >> a[i][0] >> a[i][1];
	}
	sort(a.begin() + 1, a.end());
	dp[1][1] = 0;
	for (int i = 2; i <= n; i++) { // 保留第i本书
		dp[i][1] = 0;
		for (int j = 2; j <= min(i, k); j++) { // 保留j本书
			for (int t = j - 1; t < i; t++) { // 如果选择第t本书保留
				dp[i][j] = min(dp[i][j], dp[t][j - 1] + abs(a[i][1] - a[t][1]));
			}
			
		}
	}
	for (int i = k; i <= n; i++) ans = min(ans, dp[i][k]); // 
	cout << ans;
	return 0;
}
