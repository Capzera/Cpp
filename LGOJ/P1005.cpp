#include <bits/stdc++.h>
using namespace std;
int t, n;
string base[81];
string add(string s1, string s2) {
	string ans;
	int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
	while (l >= 0 || r >= 0) {
		int a = (l >= 0 ? s1[l--] - '0' : 0);
		int b = (r >= 0 ? s2[r--] - '0' : 0);
		carry += a + b;
		ans += carry % 10 + '0';
		carry /= 10;
	}
	if (carry) ans += "1";
	reverse(ans.begin(), ans.end());
	return ans;
}
string mul(string s1, string s2) {
	if (s1 == "0" || s2 == "0") return "0";
	int l1 = s1.size(), l2 = s2.size(), x;
	string ans(l1 + l2 - 1, '0');
	for (int i = l1 - 1; i >= 0; i--) {
		x = 0;
		for (int j = l2 - 1; j >= 0; j--) {
			int a = s1[i] - '0', b = s2[j] - '0';
			x += a * b + ans[i + j] - '0';
			ans[i + j] = x % 10 + '0';
			x /= 10;
		}
		if (i) {
			ans[i - 1] += x;
		}
	}
	if (x) {
		ans = to_string(x) + ans;
	}
	return ans;
}
string max(string a, string b) {
	if (a.size() > b.size()) return a;
	if (a.size() < b.size()) return b;
	if (a >= b) return a;
	return b;
}
string ans;
int main() {
	cin >> t >> n;
	base[0] = "1";
	for (int i = 1; i <= 80; i++) {
		base[i] = mul(base[i - 1], "2");
	}
	while (t--) {
		vector<string> a(n + 1);
		for (int i = 1; i <= n; i++) cin >> a[i];
		vector<vector<string> > dp(n + 1, vector<string> (n + 1));
		for (int i = 1; i <= n; i++) dp[i][i] = mul(a[i], base[i]);
		for (int len = 1; len <= n; len++) {
			for (int i = 1; i <= n - len + 1; i++) {
				int j = i + len - 1;
				dp[i][j] = max(add(dp[i + 1][j], mul(base[n - len], a[i + 1])) , add(dp[i][j - 1], mul(base[n - len], a[j - 1])));
			}
		}
		string mx = "0";	
		for (int i = 1; i <= n; i++) mx = max(mx, add(dp[i][i], mul(a[i], base[n])));
		ans = add(ans, mx);
	}
	cout << ans;
	return 0;
}
/*
1 * 2 + 2 * 4 + 3 * 8 = 34
2 * 2 + 3 * 4 + 4 * 8 = 48
dp[i][j] = max(dp[i - 1][j] + a[i - 1] * base[n - j + i], dp[i][j + 1] + a[j + 1] * base[n - j + i])
*/
