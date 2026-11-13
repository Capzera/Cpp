#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, w, ans, vis[21];
struct edge {
	int v, w;
};
vector<edge> e[21];
void dfs(int u, int dis) {
	vis[u] = 1;
	for (edge p : e[u]) {
		int v = p.v, w = p.w;
		if (vis[v]) continue;
		dfs(v, dis + w);
	}
	ans = max(ans, dis);
	vis[u] = 0;
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		e[u].push_back({v, w});
		e[v].push_back({u, w});
	}
	for (int i = 1; i <= n; i++) {
		memset(vis, 0, sizeof(vis));
		vis[i] = 1;
		dfs(i, 0);
	}
	cout << ans;
	return 0;
}
