#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e4 + 10, M = 5e4 + 10, K = 16;
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		return w > rhs.w;
	}
} e[M];
struct dsu {
	vector<int> pa, sz;
	int n;
	void init(int _n) {
		n = _n + 1;
		pa = sz = vector<int>(n, 1);
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
		sz[x] += y;
		n--;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
} uf;
int n, m, u, v, dep[N], val[N][K], st[N][K];
vector<pii> g[N];
void dfs(int u, int fa) {
	dep[u] = dep[fa] + 1;
	st[u][0] = fa;
	for (int i = 1; i < K; i++) {
		st[u][i] = st[st[u][i - 1]][i - 1];
		val[u][i] = min(val[u][i - 1], val[st[u][i - 1]][i - 1]);
	}
	for (auto [v, w] : g[u]) {
		if (v == fa) continue;
		val[v][0] = w;
		dfs(v, u);
	}
}
int lca(int u, int v) {
	if (!uf.pd(u, v)) return -1;
	int ans = INT_MAX;
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = K - 1; ~i; i--)
		if (dep[st[u][i]] >= dep[v]) {
			ans = min(ans, val[u][i]);
			u = st[u][i];
		}
	if (u == v) return ans;
	for (int i = K - 1; ~i; i--) {
		if (st[u][i] != st[v][i]) {
			ans = min(ans, val[u][i]), u = st[u][i];
			ans = min(ans, val[v][i]), v = st[v][i];
		}
	}
	return min({ans, val[u][0], val[v][0]});
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	uf.init(n);
	for (int i = 1; i <= m; i++) cin >> e[i].u >> e[i].v >> e[i].w;
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		int u = e[i].u, v = e[i].v, w = e[i].w;
		if (uf.pd(u, v)) continue;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
		uf.merge(u, v);
	}
	for (int u = 1; u <= n; u++) {
		if (dep[u]) continue;
		dfs(u, 0);
	}
	cin >> m;
	while (m--) {
		cin >> u >> v;
		cout << lca(u, v) << '\n';
	}
	return 0;
}
