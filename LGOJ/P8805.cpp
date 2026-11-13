#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10, K = 20;
int n, m, u, v, w, a[N], dep[N], dis[N], st[N][K];
vector<int> g[N];
void dfs(int u, int fa) {
	dep[u] = dep[fa] + 1;
	st[u][0] = fa;
	dis[u] = dis[fa] + a[u];
	for (int i = 1; i < K; i++) {
		st[u][i] = st[st[u][i - 1]][i - 1];
	}
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
	}
}
int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = K - 1; ~i; i--) {
		if (dep[st[u][i]] >= dep[v]) u = st[u][i];
	}
	if (u == v) return u;
	for (int i = K - 1; ~i; i--) {
		if (st[u][i] != st[v][i])
			u = st[u][i], v = st[v][i];
	}
	return st[u][0];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
		a[u]++, a[v]++;
	}
	dfs(1, 0);
	while (m--) {
		cin >> u >> v;
		int anc = lca(u, v);
		cout << dis[u] + dis[v] - 2 * dis[anc] + a[anc] << '\n';	
	}
	return 0;
}
