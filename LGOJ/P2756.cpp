#include <bits/stdc++.h>
using namespace std;
const int N = 105;
int n, m, u, v, ans, vis[N], match[N];
vector<int> g[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v]) continue;
		vis[v] = 1;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> m >> n;
	while (cin >> u >> v, ~u && ~v) {
		g[u].push_back(v - m);
	}
	for (int u = 1; u <= m; u++) {
		memset(vis, 0, sizeof vis);
		ans += dfs(u);
	}
	cout << ans << '\n';
	for (int v = 1; v <= n - m; v++) {
		if (!match[v]) continue;
		cout << match[v] << " " << v + m << '\n';
	}
	return 0;
}
