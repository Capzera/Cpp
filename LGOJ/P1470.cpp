#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10, M = 210, p1 = 131, m1 = 1e9 + 7;
string s, str;
int n, ans, dp[N];
long long h[N], p[N];
vector<long long> hs;
vector<string> mp;
long long get(int l, int r) {
	return (h[r] - h[l - 1] * p[r - l + 1] % m1 + m1) % m1;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	p[0] = dp[0] = 1;
	for (int i = 1; i <= 200000; i++) {
		p[i] = p[i - 1] * p1 % m1;
	}
	while (cin >> s, s != ".") {
		mp.push_back(s);
		long long t = 0;
		for (auto& x : s) {
			t = (t * p1 + x) % m1;
		}
		hs.push_back(t);
	}
	s = "";
	while (cin >> str) s += str; 
	n = s.size();
	for (int i = 1; i <= n; i++) {
		h[i] = (h[i - 1] * p1 + s[i - 1]) % m1;
		for (int k = 0; k < mp.size(); k++) {
			int len = mp[k].size(), j = i - len + 1;
			if (i < len || !dp[j - 1]) continue;
			if (get(j, i) == hs[k]) {
				dp[i] = 1;
				ans = max(ans, i);
				break;
			}
		}
	}
	cout << ans;
	return 0;
}
