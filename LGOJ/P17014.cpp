#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
vector<int> g[N];
int t, n, u, v, cnt, ans, vis[N];
void dfs(int u) {
	vis[u] = 1;
	cnt++;
	for (auto& v : g[u]) {
		if (vis[v]) continue;
		dfs(v);
	}
}
int solve() {
	memset(vis, 0, sizeof(vis));
	cin >> n;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i <= n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		if (vis[u]) continue;
		cnt = 0;
		dfs(u);
		if (cnt & 1) return 3;
	}
	return 2;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
