#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
const int N = 1e6 + 5, M = 	1e6;
int n, m, a[N], b[N], mn[N], f[N];
long long ans, c;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	memset(mn, 0x3f, sizeof mn);
	for (int i = 1; i <= n; i++) {
		int d = a[i] - b[i];
		mn[a[i]] = min(mn[a[i]], d);
	}
	for (int i = 1; i <= M; i++) mn[i] = min(mn[i], mn[i - 1]);
	for (int i = 1; i <= M; i++) {
		if (mn[i] > 1e9) continue;
		f[i] = f[i - mn[i]] + 1;
	}
	int mx = mn[M];
	for (int i = 1; i <= m; i++) {
		long long k = 0;
		cin >> c;
		if (c > M) {
			k = (c - M + mx - 1) / mx;
			c -= k * mx;
		}
		ans += k + f[c];
	}
	cout << ans * 2 << '\n';
	return 0;
}
//https://codeforces.com/problemset/problem/1989/D
