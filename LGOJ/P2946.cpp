#include <bits/stdc++.h>
using namespace std;
const int mod = 1e8, N = 2e3 + 10;
long long n, f, w[N], dp[N][N];
int main() {
	cin >> n >> f;
	for (int i = 1; i <= n; i++) {
		cin >> w[i];
		w[i] %= f;
	}
	for (int i = 1; i <= n; i++) {
		dp[i][w[i]] = 1;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = f - 1; ~j; j--) {
			dp[i][j] = (dp[i][j] + dp[i - 1][j] + dp[i - 1][(j - w[i] + f) % f]) % mod;
		}
	}
	cout << dp[n][0];
	return 0;
}
