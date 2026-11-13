#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
int n, m, u, v, k, ans, vis[N], match[N];
vector<int> g[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (match[v] == -1 || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
	}
	memset(match, 0xff, sizeof match);
	for (int u = 0; u < n; u++) {
		k++;
		ans += dfs(u);
	}
	cout << n - ans;
	return 0;
}
