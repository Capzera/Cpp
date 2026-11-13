#include <bits/stdc++.h>
using namespace std;
const int N = 60;
int n, dp[N][N];
string s;
int main() {
	cin >> s; s = " " + s;
	n = s.size();
	memset(dp, 0x3f, sizeof(dp));
	for (int i = 1; i <= n; i++) dp[i][i] = 1;
	for (int len = 2; len <= n; len++) {
		for (int i = 1; i <= n - len + 1; i++) {
			int j = i + len - 1;
			if (i == 1 && j == 5) {
				cout << dp[i + 1][j] << " " << dp[i][j - 1] << endl;
			}
			if (s[i] == s[j])
				dp[i][j] = min({dp[i][j], dp[i + 1][j], dp[i][j - 1]});
			else {
				dp[i][j] = min(dp[i][j], min(dp[i + 1][j], dp[i][j - 1]) + 1);
			}
		}
	}
	cout << dp[2][5] << " " << dp[1][4] << endl;
	cout << dp[1][n];
	return 0;
}
