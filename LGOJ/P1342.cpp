#include <bits/stdc++.h>
#define pii pair<long long, long long>
using namespace std;
int n, m, u, v, w, vis[1000010];
long long ans, dis[1000010];
vector<pii> g1[1000010], g2[1000010];
void Dijkstra(vector<pii> g[]) {
	priority_queue<pii, vector<pii>, greater<pii> > q;
	memset(dis, 0x3f, sizeof(dis));
	memset(vis, 0, sizeof(vis));
	q.push({0, 1});
	dis[1] = 0;
	while (q.size()) {
		auto u = q.top().second; q.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& [v, w] : g[u]) {
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				q.push({dis[v], v});
			}
		}
	}
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g1[u].push_back({v, w});
		g2[v].push_back({u, w});
	}
	Dijkstra(g1);
	for (int i = 2; i <= n; i++) {
		ans += dis[i];
	}
	Dijkstra(g2);
	for (int i = 2; i <= n; i++) {
		ans += dis[i];
	}
	cout << ans;
	return 0;
}
