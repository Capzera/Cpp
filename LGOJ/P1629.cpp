#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e3 + 10, M = 1e5 + 10;
int n, m, u, v, vis[N];
long long ans, w, dis[N];
vector<pii> g[N];
priority_queue<pii, vector<pii>, greater<pii> > pq;
void Dijkstra(int s) {
	memset(dis, 0x3f, sizeof dis);
	memset(vis, 0, sizeof vis);
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
		g[u].push_back({v, w});
	}
	Dijkstra(1);
	for (int i = 2; i <= n; i++) ans += dis[i];
	for (int i = 2; i <= n; i++) {
		Dijkstra(i);
		ans += dis[1];
	}
	cout << ans;
	return 0;
}
