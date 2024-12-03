#include <bits/stdc++.h>
using namespace std;
string s;
int main() {
	cin >> s;
	int n = s.size();
	vector<vector<int> > dp(n, vector<int> (n, 0x3F3F3F3F));
	for (int i = 0; i < n; i++) dp[i][i] = 1;
	for (int len = 2; len <= n; len++) {
		for (int i = 0; i < n - len + 1; i++) {
			int j = i + len - 1;
			if (s[i] == s[j]) {
				dp[i][j] = min(dp[i + 1][j], dp[i][j - 1]);
				continue;
			}
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
			}
		}
	}
	cout << dp[0][n - 1];
	return 0;
}
