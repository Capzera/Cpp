#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 2e5 + 10;
vector<pii> g[N];
int n, m, s, t, u, v, vis[N], dis[N];
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	memset(vis, 0, sizeof(vis));
	memset(dis, 0x3f, sizeof(dis));
	dis[s] = 0, pq.push({0, s});
	while (pq.size()) {
		auto u = pq.top().second; pq.pop();
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
	cin >> n >> s >> t;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int u = 1; u <= n; u++) {
		cin >> m;
		for (int i = 1; i <= m; i++) {
			cin >> v;
			g[u].push_back({v, i != 1});
		}
	}
	Dijkstra(s);
	cout << (dis[t] == 0x3f3f3f3f ? -1 : dis[t]);
	return 0;
}
