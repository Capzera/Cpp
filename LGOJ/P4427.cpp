#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10, M = 21, K = 51, mod = 998244353;
int n, m, u, v, k, dep[N], st[N][M];
long long val[N][K];
vector<int> g[N];
void dfs(int u, int fa) {
	st[u][0] = fa;
	for (int i = 1; i < M; i++) 
		st[u][i] = st[st[u][i - 1]][i - 1];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		val[v][0] = 1;
		for (int i = 1; i < K; i++) val[v][i] = val[v][i - 1] * dep[v] % mod;
		for (int i = 1; i < K; i++) val[v][i] = (val[v][i] + val[u][i]) % mod;
		dfs(v, u);
	}
}
int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = M - 1; ~i; i--)
		if (dep[st[u][i]] >= dep[v]) u = st[u][i];
	if (u == v) return u;
	for (int i = M - 1; ~i; i--)
		if (st[u][i] != st[v][i]) 
			u = st[u][i], v = st[v][i];
	return st[u][0];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v), g[v].push_back(u);
	}
	dfs(1, 0);
	cin >> m;
	while (m--) {
		cin >> u >> v >> k;
		int L = lca(u, v);
		cout << ((val[u][k] + val[v][k] - val[L][k] - val[st[L][0]][k]) % mod + mod) % mod << '\n';
	}
	return 0;
}
