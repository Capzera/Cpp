#include <bits/stdc++.h>
using namespace std;
string s1, s2;
int n, m, dp[2001][2001];
int main() {
	cin >> s1 >> s2;
	n = s1.size(), m = s2.size();
	for (int i = 1; i <= n; i++) dp[i][0] = i;
	for (int j = 1; j <= m; j++) dp[0][j] = j;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			dp[i][j] = s1[i - 1] == s2[j - 1] ? dp[i - 1][j - 1] : min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
		}
	}
	cout << dp[n][m];
	return 0;
}
