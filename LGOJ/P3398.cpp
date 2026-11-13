#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int head[N], to[N << 1], nxt[N << 1], tot;
int dep[N], st[N][18];
int n, q, u, v, u1, v1, u2, v2;
vector<int> g[N];
void dfs(int u, int fa) {
	dep[u] = dep[fa] + 1;
	st[u][0] = fa;
	for (int i = 1; i <= 17; i++) {
		st[u][i] = st[st[u][i - 1]][i - 1];
	}
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
	}
}
inline int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = 17; ~i; i--) {
		if (dep[st[u][i]] >= dep[v]) {
			u = st[u][i];
		}
	}
	if (u == v) return u;
	for (int i = 17; ~i; i--) {
		if (st[u][i] != st[v][i]) 
			u = st[u][i], v = st[v][i];
	}
	return st[u][0];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> q;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	while (q--) {
		cin >> u1 >> v1 >> u2 >> v2;
		int l1 = lca(u1, v1), l2 = lca(u2, v2);
		if (dep[l1] < dep[l2]) swap(l1, l2), swap(u1, u2), swap(v1, v2);
		cout << (lca(l1, u2) == l1 || lca(l1, v2) == l1 ? "Y\n" : "N\n");
	}
	return 0;
}
