#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5, M = 1e5 + 5;
vector<int> g[N];
int n, m, u, v, c1, c2, ans, vis[N];
int dfs(int u, int color) {
	vis[u] = color;
	if (color == 1) c1++;
	else c2++;
	for (auto& v : g[u]) {
		if (vis[v] == color) return -1;
		if (vis[v]) continue;
		if (dfs(v, 3 - color) == -1) return -1;
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	while (m--) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		c1 = c2 = 0;
		if (vis[u]) continue;
		if (dfs(u, 1) == -1) return puts("Impossible"), 0;
		ans += min(c1, c2);
	}
	cout << ans;
	return 0;
}
