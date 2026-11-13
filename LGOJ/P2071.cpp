#include <bits/stdc++.h>
using namespace std;
const int N = 4e3 + 5;
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
	cin >> n;
	for (int u = 1; u <= n << 1; u++) {
		for (int i = 1; i <= 2; i++) {
			cin >> v;
			g[u].push_back(v << 1);
			g[u].push_back(v << 1 | 1);
		}
	}
	for (int u = 1; u <= n << 1; u++) {
		memset(vis, 0, sizeof vis);
		ans += dfs(u);
	}
	cout << ans;
	return 0;
}
