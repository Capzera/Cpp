#include <bits/stdc++.h>
using namespace std;
const int N = 205;
vector<int> g[N];
int n, m, k, v, ans, vis[N], match[N];
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
	cin >> n >> m;
	for (int u = 1; u <= n; u++) {
		cin >> k;
		while (k--) {
			cin >> v;
			g[u].push_back(v);
		}
	}
	for (int u = 1; u <= n; u++) {
		memset(vis, 0, sizeof vis);
		ans += dfs(u);
	}
	cout << ans;
	return 0;
}
