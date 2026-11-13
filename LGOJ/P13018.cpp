#include <bits/stdc++.h>
using namespace std;
const int N = 110, M = 5e5 + 10;
long long n, a, b, w[N], c[N], dp[N][M];
int main() {
	cin >> n;
	memset(dp, -0x3f, sizeof(dp));
	for (int i = 1; i <= n; i++) {
		cin >> a >> b;
		w[i] = a - b;
		c[i] = a + b;
	}
	dp[0][50000] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = w[i] + 100000; j >= w[i]; j--) {
			dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + c[i]);
		}
	}
	cout << dp[n][50000];
	return 0;
}
