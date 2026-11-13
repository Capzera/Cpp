#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
long long n, m, k, a[101][101], dp[101][101];
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	dp[1][1] = 1;
	for (int i = 2; i <= n; i++) {
		for (int j = 2; j <= m; j++) {
			for (int k1 = 1; k1 < i; k1++) {
				for (int k2 = 1; k2 < j; k2++) {
					if (a[k1][k2] == a[i][j]) continue;
					dp[i][j] = (dp[i][j] + dp[k1][k2]) % mod;
				}
			}
		}
	}
	cout << dp[n][m];
	return 0;
}
