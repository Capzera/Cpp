#include <bits/stdc++.h>
using namespace std;
int n, m;
long long dp[2][31];
int main() {
	cin >> n >> m;
	dp[0][0] = 1;
	for (int i = 1; i <= m; i++) {
		for (int j = 0; j < n; j++) {
			dp[i & 1][j] = dp[i & 1 ^ 1][(j - 1 + n) % n] + dp[i & 1 ^ 1][(j + 1) % n];
		}
	}
	cout << dp[m & 1][0];
	return 0;
}
/*
dp[i][j] 表示传球经过i轮，且由第j个同学拿着球的方案数
dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j + 1];
j - 1 <---- (j - 1 + n) % n, j + 1 <---- (j + 1) % n;
*/
