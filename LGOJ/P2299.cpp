#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
long long n, m, u, v, w, dis[2505], vis[2505];
vector<pii> g[2505];
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii>> pq;
	memset(vis, 0, sizeof(vis));
	memset(dis, 0x3f, sizeof(dis));
	pq.push({0, s}), dis[s] = 0;
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
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	Dijkstra(1);
	cout << dis[n];
	return 0;
}
