#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int n, m, l, v, c, ans = -1, dp[N][2], col[N][2];
int main() {
	cin >> n >> m;
	memset(dp, 0xff, sizeof dp);
	dp[0][1] = col[0][1] = 0, dp[0][0] = col[0][0] = -1;
	for (int i = 1; i <= n; i++) {
		cin >> l >> v >> c;
		for (int j = m; j >= l; j--) {
			int val = -1;
			if (dp[j - l][1] != -1 && col[j - l][1] != c) val = dp[j - l][1] + v;
			else if (dp[j - l][0] != -1 && col[j - l][0] != c) val = dp[j - l][0] + v;
			if (val == -1) continue;
			if (val > dp[j][1]) {
				if (c != col[j][1]) {
					dp[j][0] = dp[j][1];
					col[j][0] = col[j][1];
					col[j][1] = c;
				}
				dp[j][1] = val;
			} else if (val > dp[j][0] && c != col[j][1]) {
				dp[j][0] = val;
				col[j][0] = c;
			}
		}
	}
	cout << max(dp[m][0], dp[m][1]);
	return 0;
}
