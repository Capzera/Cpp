#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 5e5 + 10, K = 20;

int n, m, root, u, v, w, dep[N], vis[N], dis[N], st[N][K];
vector<pii> g[N];
void dfs(int u, int fa) {
	dep[u] = dep[fa] + 1;
	st[u][0] = fa;
	for (int i = 1; i < K; i++) {
		st[u][i] = st[st[u][i - 1]][i - 1];
	}
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
	}
}
void Dijkstra() {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	memset(dis, 0x3f, sizeof(dis));
	dis[1] = 0;
	pq.push({0, 1});
	while (pq.size()) {
		int u = pq.top().second; pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& [v, w] : g[u]) {
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				pq.push({dis[v], v});
			}
		}
	}
}
int lca(int u, int v) { 
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = K - 1; ~i; i--) {
		if (dep[st[u][i]] >= dep[v]) u = st[u][i];
	}
	if (u == v) return u;
	for (int i = K - 1; ~i; i--) {
		if (st[u][i] != st[v][i])
			u = st[u][i], v = st[v][i];
	}
	return st[u][0];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	Dijkstra();
	dfs(1, 0);
	while (m--) {
		cin >> u >> v;
		cout << dis[u] + dis[v] - 2 * dis[lca(u, v)] << '\n';	
	}
	return 0;
}
