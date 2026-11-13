#include <bits/stdc++.h>
using namespace std;
const int N = 505;
vector<int> g[N];
int n, m, e, u, v, ans, vis[N], match[N];
bool dfs(int u) {
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
	cin >> n >> m >> e;
	for (int i = 1; i <= e; i++) {
		cin >> u >> v;
		g[u].push_back(v);
	}
	for (int i = 1; i <= n; i++) {
		memset(vis, 0, sizeof(vis));
		ans += dfs(i);
	}
	cout << ans << "\n";
	return 0;
}
