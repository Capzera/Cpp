#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 3;
int n, ans = INT_MIN, a[N], b[N], c[N], dp[N][N][3];
int get(int i, int x) {
	if (x == (c[i] + 1) % 3) return a[i] << 1;
	if (x == c[i]) return a[i];
	return 0;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) cin >> b[i], b[i] += b[i - 1];
	for (int i = 1; i <= n; i++) cin >> c[i];
	memset(dp, 0xc0, sizeof(dp));
	for (int k = 0; k < 2; k++) dp[1][0][k] = get(1, k);
	for (int i = 2; i <= n; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < 3; k++) {
				if (j != i - 1) dp[i][j][k] = max(dp[i][j][k], dp[i - 1][j][k]);
				if (j) dp[i][j][k] = max(dp[i][j][k], max(dp[i - 1][j - 1][(k + 1) % 3], dp[i - 1][j - 1][(k + 2) % 3])); 
				dp[i][j][k] += get(i, k);
			}
		}
	}
	for (int j = 0; j < n; j++)
		for (int k = 0; k < 3; k++) 
			ans = max(ans, dp[n][j][k] - b[j]);
	cout << ans;
	return 0;
}
