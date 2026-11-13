#include <bits/stdc++.h>
using namespace std;
long long n, m;
string s, a[41][41], dp[41][7];
string Max(string a, string b) {
	int l1 = a.size(), l2 = b.size();
	return l1 > l2 || l1 == l2 && a > b ? a : b;
}
string mul(string a, string b) {
	if (a == "0" || b == "0") return "0";
	int l1 = a.size(), l2 = b.size(), x = 0;
	string ans(l1 + l2 - 1, '0');
	for (int i = l1 - 1; i >= 0; i--) {
		x = 0;
		for (int j = l2 - 1; j >= 0; j--) {
			int c1 = a[i] - '0', c2 = b[j] - '0';
			x += c1 * c2 + ans[i + j] - '0';
			ans[i + j] = x % 10 + '0';
			x /= 10;
		}
		if (i) ans[i - 1] += x;
	}
	if (x) ans = to_string(x) + ans;
	return ans;
}
int main() {
	cin >> n >> m >> s;
	s = ' ' + s;
	for (int i = 1; i <= n; i++)
		for (int j = i; j <= n; j++)
			a[i][j] = a[i][j - 1] + s[j];
	for (int i = 1; i <= n; i++) dp[i][0] = a[1][i];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (j >= i) break;
			for (int k = j; k < i; k++) {
				dp[i][j] = Max(dp[i][j], mul(dp[k][j - 1], a[k + 1][i]));
			}
		}
	}
	string ans = dp[n][m];
	reverse(ans.begin(), ans.end());
	while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
	reverse(ans.begin(), ans.end());
	cout << ans;
	return 0;
}
/*
dp[i][j] 表示前i个位置放j个乘号获得的最大乘积
分割点为k
dp[i][j] = max(dp[i][k] * dp[k + 1][j];
*/
