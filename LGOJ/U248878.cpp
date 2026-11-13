#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int n, m, u, v, ans, vis[N];
vector<int> g[N];
int dfs(int u, int color) {
	vis[u] = color;
	for (auto& v : g[u]) {
		if (vis[v] == color) return 0;
		if (!vis[u] && !dfs(v, 3 - color)) return 0;
	}
	return 1;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		if (!vis[u] && !dfs(u, 1)) return puts("No"), 0;
	}
	puts("Yes");
	return 0;
}
