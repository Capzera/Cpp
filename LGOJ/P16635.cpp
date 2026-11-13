#include <bits/stdc++.h>
using namespace std;
const int N = 110, M = 2e4 + 10;
int t, n, m;
long long r[N], b[N];
struct dsu {
	vector<int> pa, sz;
	int n;
	dsu(int _n) {
		n = _n;
		pa = sz = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (sz[x] < sz[y]) swap(x, y);
		pa[y] = x;
		sz[x] += sz[y];
		n--;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
struct edge {
	int u, v;
	long long w;
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
} e[M];
long long solve() {
	cin >> n;
	long long k = 0, ans = 0;
	for (int i = 1; i <= n; i++) cin >> r[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j < i; j++) {
			e[++k] = {i, j, r[i] ^ b[j]};
			e[++k] = {i, j, r[j] ^ b[i]};
		}
	}
	struct dsu uf(n);
	sort(e + 1, e + k + 1);
	for (int i = 1; i <= k; i++) {
		int u = e[i].u, v = e[i].v;
		long long w = e[i].w;
		if (uf.pd(u, v)) continue;
		ans += w;
		uf.merge(u, v);
		if (uf.n == 1) break;
	}
	return ans;
}
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
