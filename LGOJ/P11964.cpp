#include <bits/stdc++.h>
using namespace std;
int n, m, k, u, v, vis[501][501];
vector<int> g[501];
void dfs(int u, int cnt) {
	if (cnt == k) return;
	for (auto& v : g[u]) {
		if (vis[cnt + 1][v]) continue;
		vis[cnt + 1][v] = 1;
		dfs(v, cnt + 1);
	}
}
int main() {
	cin >> n >> m >> k;
	while (m--) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		memset(vis, 0, sizeof(vis));
		dfs(u, 0);
		for (int i = 1; i <= k; i++) {
			int ans = 0;
			for (int j = 1; j <= n; j++) {
				ans += vis[i][j];
			}
			cout << ans << " ";
		}
		puts("");
	}
	return 0;
}
