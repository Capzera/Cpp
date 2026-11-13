#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, p = 19650827;
long long n, a[N], dp[N][N][2];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i][i][0] = 1;
	}
	for (int len = 1; len <= n; len++) {
		for (int i = 1; i <= n - len + 1; i++) {
			int j = i + len - 1;
			if (a[i] < a[i + 1]) dp[i][j][0] += dp[i + 1][j][0];
			if (a[i] < a[j]) {
				dp[i][j][0] += dp[i + 1][j][1];
				dp[i][j][1] += dp[i][j - 1][0];
			}
			if (a[j] > a[j - 1]) dp[i][j][1] += dp[i][j - 1][1];
			dp[i][j][0] %= p;
			dp[i][j][1] %= p;
		}
	}
	cout << (dp[1][n][0] + dp[1][n][1]) % p;
	return 0;
}
