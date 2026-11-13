#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 10, M = 8e6 + 9e3;
long long A, B, n, m, k, ans, a[N], b[N];
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
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> A >> B >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) cin >> b[i];
	sort(a + 1, a + n + 1);
	sort(b + 1, b + m + 1);
	a[++n] = A, b[++m] = B;
	struct dsu uf(n * m);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (j < m) e[++k] = {(i - 1) * m + j - 1, (i - 1) * m + j, a[i] - a[i - 1]};
			if (i < n) e[++k] = {(i - 1) * m + j - 1, i * m + j - 1, b[j] - b[j - 1]};
		}
	}
	sort(e + 1, e + k + 1);
	for (int i = 1; i <= k; i++) {
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
