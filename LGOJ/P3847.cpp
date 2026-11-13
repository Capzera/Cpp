#include <bits/stdc++.h>
using namespace std;
const int N = 3010;
int n, a[N], dp[N][N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i <= n - len + 1; i++) {
			int j = i + len - 1;
			if (a[i] == a[j]) dp[i][j] = dp[i + 1][j - 1];
			else dp[i][j] = min({dp[i + 1][j], dp[i][j - 1], dp[i + 1][j - 1]}) + 1;
		}
	}
	cout << dp[1][n];
	return 0;
}
/*
设dp[i][j] 表示 [i..j] 调整的最小次数
if (a[i] == a[j]) dp[i][j] = dp[i + 1][j - 1];
else dp[i][j] = min({dp[i + 1][j], dp[i][j - 1], dp[i + 1][j - 1]}) + 1;
*/
