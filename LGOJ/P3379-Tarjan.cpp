#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, m, r, u, v;
vector<int> g[N];
vector<pair<int, int>> queries[N];
int ans[N], fa[N], vis[N];
int find(int x) {
	return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void tarjan(int u) {
	vis[u] = 1;
	for (int v : g[u]) {
		if (vis[v]) continue;
		tarjan(v);
		fa[v] = u;
	}
	for (auto [v, id] : queries[u]) {
		if (vis[v] == 2) {
			ans[id] = find(v);
		}
	}
	vis[u] = 2;
}
int main() {
	scanf("%d%d%d", &n, &m, &r);
	for (int i = 1; i <= n; i++) fa[i] = i;
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v);
		if (u == v) ans[i] = u;
		queries[u].push_back({v, i});
		queries[v].push_back({u, i});
	}
	tarjan(r);
	for (int i = 0; i < m; i++) printf("%d\n", ans[i]);
	return 0;
}

