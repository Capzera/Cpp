#include <bits/stdc++.h>
using namespace std;
const int N = 101;
vector<int> g[N];
int n, u, v, mxd, mxw;
int fa[N], dep[N], cnt[N];
void dfs(int u, int fa) {
	mxd = max(mxd, dep[u]);
	mxw = max(mxw, ++cnt[dep[u]]);
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u);
	}
}
int lca(int x, int y) {
	while (x != y) {
		if (dep[x] >= dep[y]) {
			x = fa[x];
		} else {
			y = fa[y];
		}
	}
	return x;
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		fa[v] = u;
	}
	cin >> u >> v;
	dfs(1, 0);
	int w = lca(u, v);
	printf("%d\n%d\n%d", mxd + 1, mxw, 2 * dep[u] + dep[v] - 3 * dep[w]);
	return 0;
}
