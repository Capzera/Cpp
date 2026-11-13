#include <bits/stdc++.h>
using namespace std;
const int N = 301, M = 1e5 + 10;
long long s, n, x, k, ans, w[N];
struct dsu {
	vector<int> pa, sz;
	int n;
	dsu (int _n) {
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
		pa[y] = x;
		sz[x] += sz[y];
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
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		e[++k] = {i, n + 1, x};
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> x;
			if (j <= i) continue;
			e[++k] = {i, j, x};
		}
	}
	dsu uf(n + 1);
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
