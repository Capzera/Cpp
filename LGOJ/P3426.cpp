#include <bits/stdc++.h>
using namespace std;
string s;
int n;
unordered_map<int, int> mp;
int main() {
	cin >> s;
	n = s.size();
	s = " " + s;
	vector<int> pi(n + 1), dp(n + 1);
	for (int j = 2, k = 0; j <= n; j++) {
		while (k && s[j] != s[k + 1]) k = pi[k];
		if (s[j] == s[k + 1]) k++;
		pi[j] = k;
	}
	dp[1] = 1, mp[1] = 1;
	for (int i = 2; i <= n; i++) {
		dp[i] = i;
		if (mp[dp[pi[i]]] >= i - pi[i]) dp[i] = dp[pi[i]];
		mp[dp[i]] = i;
	}
	cout << dp[n];
	return 0;
}
