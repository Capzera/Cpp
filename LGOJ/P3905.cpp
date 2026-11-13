#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e2 + 10, M = 5e3 + 10;
int n, m, u, v, w, s, t, a[N][N], vis[N];
long long dis[N];
vector<pii> g[N];
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	memset(dis, 0x3f, sizeof dis);
	dis[s] = 0;
	pq.push({0, s});
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
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, 0});
		g[v].push_back({u, 0});
		a[u][v] = a[v][u] = w;
	}
	cin >> m;
	while (m--) {
		cin >> u >> v;
		for (auto& [vv, w] : g[u]) {
			if (vv == v) w = a[u][v];
		}
		for (auto& [uu, w] : g[v]) {
			if (uu == u) w = a[u][v];
		}
	}
	cin >> s >> t;
	Dijkstra(s);
	cout << dis[t];
	return 0;
}
