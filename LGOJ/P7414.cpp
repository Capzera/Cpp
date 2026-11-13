#include <bits/stdc++.h>
using namespace std;
int n, a[301], dp[301][301];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	
	for (int i = 1; i <= n; i++) dp[i][i] = 1;
	
	for (int len = 2; len <= n; len++) { // 枚举长度
		for (int i = 1; i <= n - len + 1; i++) { // 枚举起点
			int j = i + len - 1; // 起点 + 长度 - 1 = 终点
			
			if (a[i] == a[j]) {
				dp[i][j] = min(dp[i + 1][j], dp[i][j - 1]);
				continue;
			}
			
			dp[i][j] = INT_MAX;
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
			}
		}
	}
	cout << dp[1][n];
	return 0;
}
