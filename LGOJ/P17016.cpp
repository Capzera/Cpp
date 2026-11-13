#include <bits/stdc++.h>
using namespace std;
struct point {
	long long x, y;
	void read() {cin >> x >> y;}
} a[501];
struct dsu {
	vector<int> pa, sz;
	dsu(int n) {
		pa = sz = vector<int>(n + 1, 1);
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
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
};
long long dis(point a, point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
int n, m;
double ans;
vector<edge> e;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) a[i].read();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			e.push_back({i, j, dis(a[i], a[j])});
	struct dsu uf(n);
	sort(e.begin(), e.end());
	for (auto& g : e) {
		if (uf.pd(g.u, g.v)) continue;
		if (g.w > m * m) return puts("Impossible"), 0;
		uf.merge(g.u, g.v);
		ans += sqrt(g.w);
	}
	printf("%.2lf", ans);
	return 0;
}
