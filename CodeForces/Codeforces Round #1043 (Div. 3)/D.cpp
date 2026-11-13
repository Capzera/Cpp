#include <bits/stdc++.h>
using namespace std;
long long n, c[16];
void solve() {
	cin >> n;
	long long cnt = 9, len = 1, k = 0, ans = 0;
	while (n >= cnt) {
		n -= cnt;
		k += cnt;
		cnt *= 10;
		len++;
	}
	c[0] = 5;
	for (int i = 1; i <= 15; i++) c[i] = c[i - 1] * 9;
	k += n / len;
	string s = to_string(k + 1);
	for (int i = 0; i < n; i++) ans += s[i] - '0';
	s = to_string(k);
	for (int i = 0, base = 1; i < s.size(); i++, base *= 10) {
		int lo = s.back() - '0', hi = s[0] - '0', cur = s[i] - '0';
		for (int x = 1; x <= 9; x++) {
			if (x < cur) ans += x * (hi + 1) * base;
			if (x == cur) ans += x * (hi * base + lo + 1);
			if (x > cur ) ans += x * hi * base;
		}
	}
	cout << ans << '\n';
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	return 0;
}
