#include <bits/stdc++.h>
using namespace std;
string s;
int n, ans, j, dp[1000010];
unordered_map<char, char> mp = {{')', '('}, {']', '['}};
int main() {
	cin >> s;
	n = s.size();
	for (int i = 1; i < n; i++) {
		if (s[i] == '(' || s[i] == '[') continue;
		if (s[i - 1 - dp[i - 1]] != mp[s[i]]) continue;
		dp[i] = dp[i - 1] + 2 + dp[i - 1 - dp[i - 1] - 1];
		if (dp[i] > ans) {
			ans = dp[i], j = i - dp[i] + 1;
		}
	}
	cout << s.substr(j, ans);
	return 0;
}
