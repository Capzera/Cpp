#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10, M = 2e5 + 10;
struct dsu {
	vector<int> pa, sz;
	int n;
	dsu(int _n) {
		n = _n;
		pa = sz = vector<int> (n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return x == pa[x] ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (sz[x] < sz[y]) swap(x, y);
		sz[x] += sz[y];
		pa[y] = x;
		n--;
	}
	int pd(int x, int y) {
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
int n, m, a[N];
long long ans;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	struct dsu uf(n);
	for (int i = 1; i <= n; i++) cin >> a[i];
	ans = *min_element(a + 1, a + n + 1);
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u >> e[i].v >> e[i].w;
		e[i].w = 2 * e[i].w + a[e[i].u] + a[e[i].v];
	}
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		int u = e[i].u, v = e[i].v;
		long long w = e[i].w;
		if (uf.pd(u, v)) continue;
		uf.merge(u, v);
		ans += w;
		if (uf.n == 1) break;
	}
	cout << ans;
	return 0;
}
