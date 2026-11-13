#include <bits/stdc++.h>
using namespace std;
const int N = 2010;
int t;
string s;
long long dp[N], f[N], g[N];
long long solve() {
	cin >> s;
	long long n = s.size(), ans = 0;
	memset(f, 0, sizeof(f));
	memset(g, 0, sizeof(g));
	for (int len = 1; len * 2 <= n; len++) {
		for (int i = 0; i + len * 2 <= n; i++) {
			if (s.substr(i, len) == s.substr(i + len, len)) {
				f[i + 2 * len - 1]++;
				g[i]++;
			}
		}
	}
	for (int i = 0; i < n - 1; i++) {
		ans += 1ll * f[i] * g[i + 1];
	}
	return ans;
}

int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
