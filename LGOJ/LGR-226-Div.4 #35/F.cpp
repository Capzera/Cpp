#include <bits/stdc++.h>
using namespace std;
string s;
void solve() {
	cin >> s;
	int n = s.size(), ans = n;
	vector<vector<int> > dp(n, vector<int>(n));
	for (int i = 0; i < n; i++) dp[i][i] = 1; // 当只有单个字符时，一定是通配回文子串
	for (int len = 2; len <= n; len++) {
		for (int i = 0; i < n - len + 1; i++) {
			int j = i + len - 1;
			dp[i][j] = (len == 2 || dp[i + 1][j - 1] == 1) && (s[i] == s[j] || s[i] == '?' || s[j] == '?');
			ans += dp[i][j];
		}
	}
	cout << ans << endl;
}

int main() {
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}
/*
设 dp[i][j] 表示对于字符串s下标[i..j]是否是一个通配回文子串
cout << sum(dp[i][j]);
dp[i][i] = 1
当串长度为2时，单独处理
dp[i][j] = 1 (len == 2 || dp[i + 1][j - 1] = 1) && (s[i] == s[j] || s[i] == '?' || s[j] == '?')
dp[i][j] = 0 否则








*/
