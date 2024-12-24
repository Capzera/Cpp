#include <bits/stdc++.h>
using namespace std;
int T, n, k, dp[8][101];
int main() {
	cin >> T;
	dp[0][0] = 1;
	for (int i = 1; i <= 7; i++)
		for (int j = 0; j <= 100; j++)
 			for (int p = 0; p <= min(9, j); p++)
				dp[i][j] += dp[i - 1][j - p];
	while (T--) {
		cin >> n >> k;
		long long ans = 0;
		for (int j = 1; j <= k; j++) ans += dp[n][j];
		cout << ans << endl;
	}
	return 0;
}
