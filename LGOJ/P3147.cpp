#include <bits/stdc++.h>
using namespace std;
int n, x, dp[59][262150], ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		dp[x][i] = i + 1;
	}
	for (int i = 2; i <= 58; i++) {
		for (int j = 1; j <= n; j++) {
			if (!dp[i][j]) dp[i][j] = dp[i - 1][dp[i - 1][j]];
			if (dp[i][j]) ans = max(ans, i);
		}
	}
	cout << ans;
	return 0;
}
/*
设dp[i][j] 表示 以j作为左端点合并出i的右端点
dp[a[i]][i] = 
dp[i][j] = 0 表示以j作为左端点无法合并出i
dp[i][j] = dp[dp[i - 1][j]][j]
dp[1][1] = 2
dp[1][2] = 3
dp[1][3] = 4
dp[2][4] = 5
dp[2][1] = 3
dp[2][2] = 4
dp[3][2] = 5
*/
