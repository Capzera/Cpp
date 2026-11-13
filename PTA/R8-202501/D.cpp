#include <bits/stdc++.h>
using namespace std;
string s;
int n, dp[51][51];
int main() {
	cin >> s;
	n = s.size();
	for (int i = 0; i <= n; i++) dp[i][i] = 1;
	for (int len = 2; len <= n; len++) {
		for (int i = 0; i < n - len + 1; i++) {
			int j = i + len - 1;
			if (s[i] == s[j]) {
				dp[i][j] = min(dp[i][j - 1], dp[i + 1][j]);
			} else {
				dp[i][j] = 0x3f3f3f3f;
				for (int k = i; k < j; k++) {
					dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
				}
			}
		}
	}
	cout << dp[0][n - 1];
	return 0;
}
