#include <bits/stdc++.h>
using namespace std;
const int N = 201;
int T, n, x, vis[N], match[N];
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
int solve() {
	int ans = 0;
	cin >> n;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> x;
			if (x) g[i].push_back(j);
		}
	}
	memset(match, 0, sizeof match);
	for (int u = 1; u <= n; u++) {
		memset(vis, 0, sizeof vis);
		ans += dfs(u);
	}
	return ans == n;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> T;
	while (T--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
