#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5, M = 2e5 + 5;
int n, m, u, v, wt, bl, mn, mx, vis[N];
vector<int> g[N];
void dfs(int u, int fa, int color) {
	vis[u] = 1;
	color ? wt++ : bl++;
	for (auto& v : g[u]) {
		if (vis[v]) continue;
		dfs(v, u, !color);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		wt = bl = 0;
		dfs(i, 0, 1);
		mn += min(wt, bl);
		mx += max(wt, bl);
	}
	cout << mn << " " << mx;
	return 0;
}
