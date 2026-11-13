#include <bits/stdc++.h>
#define pdi pair<double, int>
using namespace std;
const int N = 2e3 + 10, M = 1e5 + 10;
int n, m, u, v, s, t, vis[N];
double w, dis[N];
vector<pdi> g[N];
void Dijkstra(int s) {
	priority_queue<pdi> pq;
	dis[s] = 1;
	pq.push({1, s});
	while (pq.size()) {
		int u = pq.top().second; pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& [w, v] : g[u]) {
			if (dis[u] * w  > dis[v]) {
				dis[v] = dis[u] * w;
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
		g[u].push_back({1.0 - w / 100.0, v});
		g[v].push_back({1.0 - w / 100.0, u});
	}
	cin >> s >> t;
	Dijkstra(s);
	printf("%.8lf", 100.0 / dis[t]);
	return 0;
}
