#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e3 + 10, M = 1e5 + 10;
int n, m, u, v;
long long ans, w;
vector<pii> g1[N], g2[N];
vector<long long> vis, d1, d2;
void Dijkstra(int s, vector<pii> g[N], vector<long long> &dis) {
	dis = vector<long long>(n + 1, LLONG_MAX);
	vis = vector<long long>(n + 1, 0);
	priority_queue<pii, vector<pii>, greater<pii> > pq;
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
		g1[u].push_back({v, w});
		g2[v].push_back({u, w});
	}
	Dijkstra(1, g1, d1);
	Dijkstra(1, g2, d2);
	for (int i = 2; i <= n; i++) ans += d1[i] + d2[i];
	cout << ans;
	return 0;
}
