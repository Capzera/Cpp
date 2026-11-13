#include <bits/stdc++.h>
#include <conio.h>
#define ll long long
#define pii pair<ll, ll>
using namespace std;
const int N = 1e6 + 10;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return f * ans;
}

int n, m, q, x, y, z;

void solve() {
	n = read(), m = read(), q = read();
	vector<ll> a(n), b(m), s1(n + 1), s2(m + 1);
	for (auto& x : a) x = read();
	for (auto& x : b) x = read();
	sort(a.begin(), a.end(), greater<ll>()), sort(b.begin(), b.end(), greater<ll>());
	for (int i = 1; i <= n; i++) s1[i] = s1[i - 1] + a[i - 1];
	for (int i = 1; i <= m; i++) s2[i] = s2[i - 1] + b[i - 1];
	while (q--) {
		cin >> x >> y >> z;
		int L = max(0, z - y), R = min(x, z);
		if (L == R) {
			cout << s1[L] + s2[z - L] << "\n";
			continue;
		}
		int l = L, r = R;
		while (r - l > 2) {
			int m1 = l + (r - l) / 3;
			int m2 = r - (r - l) / 3;
			ll f1 = s1[m1] + s2[z - m1];
			ll f2 = s1[m2] + s2[z - m2];
			if (f1 < f2) {
				l = m1;
			} else {
				r = m2;
			}
		}
		ll ans = 0;
		for (int i = l; i <= r; i++) {
			ans = max(ans, s1[i] + s2[z - i]);
		}
		cout << ans << '\n';
	}
}

int main() {
	int t = 1;
	cin >> t;
	while (t--) solve();
	// while(t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
