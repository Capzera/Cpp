#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 5;
int n, m, u, v, k, ans, col[N], vis[N], match[N];
vector<int> g[N];
void dfs(int u) { // 给二分图染色
	for (auto& v : g[u]) {
		if (col[v] != -1) continue;
		col[v] = col[u] ^ 1;
		dfs(v);
	}
}
int Hungarian(int u) { // 匈牙利
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (match[v] == -1 || Hungarian(match[v])) {
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
		g[v].push_back(u);
	}
	memset(col, 0xff, sizeof col);
	for (int u = 0; u < n; u++) {
		if (col[u] != -1) continue;
		col[u] = 0;
		dfs(u);
	}
	memset(match, 0xff, sizeof match);
	for (int u = 0; u < n; u++) {
		if (col[u] != 0) continue;
		k++;
		ans += Hungarian(u);
	}
	cout << n - ans;
	return 0;
}
