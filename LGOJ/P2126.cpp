#include <bits/stdc++.h>
using namespace std;
struct graph {
	int u, v, w;
	bool operator < (const struct graph& rhs) const {
		return w < rhs.w;
	}
} a[400001];
struct dsu {
	int cnt, n;
	vector<int> size, pa;
	dsu(int _n) : cnt(_n) {
		size = pa = vector<int>(_n, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return x == pa[x] ? pa[x] : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		size[x] += size[y];
		pa[y] = x;
		cnt--;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
int n, m, ans;
int main() {
	cin >> n >> m;
	struct dsu uf(n + 1);
	for (int i = 1; i <= m; i++) {
		cin >> a[i].u >> a[i].v >> a[i].w;
	}
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		if (uf.pd(a[i].u, a[i].v)) continue;
		ans += a[i].w;
		uf.merge(a[i].u, a[i].v);
		if (!uf.cnt) break;
	}
	cout << ans;
	return 0;
}
