#include <bits/stdc++.h>
using namespace std;
long long n, s, mx;
int dp[51][1001], a[51];
int main() {
	cin >> n >> s >> mx;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	dp[0][s] = 1;
	for (int i = 1; i <= n; i++) {
		for (int j = mx; j >= 0; j--) {
			if (j - a[i] >= 0) {
				dp[i][j] |= dp[i - 1][j - a[i]];
			}
			if (j + a[i] <= mx) {
				dp[i][j] |= dp[i - 1][j + a[i]];
			}
		}
	}
	for (int i = mx; i >= 1; i--) {
		if (dp[n][i]) {
			cout << i;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
