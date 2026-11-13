#include <bits/stdc++.h>
#define pii pair<long long, long long>
using namespace std;
int n, m, s, q, u, v, w, vis[2000010];
long long dis[200010];
vector<pii> g[200010];
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > q;
	memset(dis, 0x3f, sizeof(dis));
	q.push({0, s});
	dis[s] = 0;
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
	cin >> n >> m >> s >> q;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	Dijkstra(s);
	while (q--) {
		cin >> u;
		cout << dis[u] << endl;
	}
	return 0;
}
