#include <bits/stdc++.h>
using namespace std;
const int N = 205, M = 6e3 + 5;
int n, m, u, v, w, ans[M], vis[M];
struct dsu {
	vector<int> pa, sz;
	int count;
	dsu(int _n) {
		count = _n;
		pa = sz = vector<int>(count + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return x == pa[x] ? x : x = find(pa[x]);
	}
	void merge(int u, int v) {
		u = find(u), v = find(v);
		if (u == v) return;
		if (sz[u] < sz[v]) swap(u, v);
		pa[v] = u;
		sz[u] += sz[v];
		count--;
	}
	bool pd(int u, int v) {
		return find(u) == find(v);
	}
	int size(int x) {
		return sz[find(x)];
	}
};
struct edge {
	int u, v, w, id;
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
} e[M], g[M];
int Kruskal(int k) {
	memset(vis, 0, sizeof vis);
	struct dsu uf(n);
	for (int i = 1; i <= k; i++) g[i] = e[i];
	sort(g + 1, g + k + 1);
	long long ans = 0;
	for (int i = 1; i <= k; i++) {
		int u = g[i].u, v = g[i].v, w = g[i].w, id = g[i].id;
		if (uf.pd(u, v)) continue;
		uf.merge(u, v);
		ans += w;
		vis[id] = 1;
		if (uf.count == 1) return ans;
	}
	return -1;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	memset(ans, 0xff, sizeof ans);
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u >> e[i].v >> e[i].w;
		e[i].id = i;
	}
	ans[m] = Kruskal(m);
	for (int i = m; i > 1; i--) {
		if (ans[i] == -1) break;
		if (!vis[i]) {
			ans[i - 1] = ans[i];
		} else {
			ans[i - 1] = Kruskal(i - 1);
		}
	}
	for (int i = 1; i <= m; i++) cout << ans[i] << '\n';
	return 0;
}
