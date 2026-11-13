#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10, mod = 1e9 + 7;
string s, p;
int t, lens, lenp;
long long dp[N];
vector<int> kmp(string s) {
	vector<int> next(s.size() + 1);
	int j = 0, k = next[0] = -1;
	while (j < s.size()) {
		if (k == -1 || s[j] == s[k]) {
			j++, k++;
			next[j] = k;
		} else k = next[k];
	}
	return next;
}
long long solve() {
	cin >> s >> p;
	lens = s.size(), lenp = p.size();
	vector<int> next = kmp(p);
	dp[0] = 1;
	int i = 0, j = 0;
	while (i < lens) {
		if (j == lenp) {
			dp[i] = (dp[i] + dp[i - lenp]) % mod;
			j = next[j];
			continue;
		}
		if (j == -1 || s[i] == p[j]) {
			i++, j++;
			dp[i] = i < lenp ? 1 : dp[i - 1];
		} else j = next[j];
	}
	if (j == lenp) return (dp[lens] + dp[lens - lenp]) % mod;
	return dp[lens];
}

int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
