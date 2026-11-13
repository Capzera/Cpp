#include <bits/stdc++.h>
using namespace std;
const int N = 2010, mod = 998244353;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
struct node {
	long long a, id;
	bool operator < (const struct node& rhs) const {
		if (a != rhs.a) return a > rhs.a;
		return id < rhs.id;
	}
} vec[N];
long long t, n, k;
long long a[N], b[N], vis[N], chk[N];

void solve() {
	n = read(), k = read();
	long long ans = 0;
	memset(vis, 0, sizeof(vis));
	memset(chk, 0, sizeof(chk));
	for (int i = 1; i <= n; i++) {
		a[i] = read();
		vec[i] = {a[i], i};
		vis[i] = 0;
	}
	if (n > 30) {
		cout << 0 << '\n';
		return;
	}
	sort(vec + 1, vec + n + 1);
	for (int i = 1; i <= k; i++) {
		vis[vec[i].id] = 1;
	}
	for (int mask = 0; mask < 1 << n; mask++) {
		long long fz = 0, fm = 0;
		for (int i = 0; i < n; i++) {
			b[i + 1] = (mask >> i & 1) + 1;
			if (vis[i + 1]) {
				fz += a[i + 1];
				fm += b[i + 1];
			}
		}
		for (int i = 1; i <= n; i++) {
			chk[i] = fm * a[i] - fz * b[i];
		}
		sort(chk + 1, chk + n + 1, greater<long long>());
		long long mx = 0;
		for (int i = 1; i <= k; i++) mx += chk[i];
		if (mx <= 0) {
			ans = (ans + 1) % mod;
		}
	}
	cout << ans << '\n';
}
int main() {
	t = read();
	while (t--) solve();
	return 0;
}
