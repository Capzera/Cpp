#include <bits/stdc++.h>
using namespace std;
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		return w > rhs.w;
	}
} a[100001];
struct dsu {
	vector<int> pa;
	dsu(int n) {
		pa = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		pa[x] = y;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
int n, m, u, v, w;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) cin >> a[i].u >> a[i].v >> a[i].w;
	sort(a + 1, a + m + 1);
	dsu uf(2 * n);
	for (int i = 1; i <= m; i++) {
		int u = a[i].u, v = a[i].v;
		uf.merge(u + n, v), uf.merge(u, v + n);
		if (uf.pd(u, u + n) || uf.pd(v, v + n)) {
			cout << a[i].w;
			return 0;
		}
	}
	cout << 0;
	return 0;
}
