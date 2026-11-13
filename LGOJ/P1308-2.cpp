#include <bits/stdc++.h>
using namespace std;
const int P = 131, M = 998244353, N = 1e6 + 10;
long long hs, ans, loc = -1, p[N], h[N];
string s, t;
int main() {
	getline(cin, t);
	getline(cin, s);
	int n = s.size(), m = t.size();
	p[0] = 1;
	for (int i = 1; i <= n; i++) p[i] = p[i - 1] * P % M;
	for (int i = 0; i < t.size(); i++) {
		hs = (hs * P + tolower(t[i])) % M;
	}
	for (int i = 1; i <= n; i++) {
		h[i] = (h[i - 1] * P + tolower(s[i - 1])) % M;
	}
	for (int r = m; r <= n; r++) {
		long long x = (h[r] - h[r - m] * p[m] + M) % M;
		if (x == hs) {
			if (loc == -1) loc = r - m;
			ans++;
		}
	}
	if (!ans) return puts("-1"), 0;
	cout << ans << " " << loc;
	return 0;
}
