#include <bits/stdc++.h>
#define pii pair<long long, int>
#define int long long
using namespace std;
const int N = 200002;
int n, m, u, v, w, S, T, dis[N], vis[N];
vector<pii> g[N];
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
signed main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		v += v > n ? -n : n;
		g[u].push_back({v, w});
		v += v > n ? -n : n;
		u += u > n ? -n : n;
		g[v].push_back({u, w});
	}
	cin >> S >> T;
	S += S > n ? -n : n;
	Dijkstra(S);
	if (dis[T] == 0x3f3f3f3f3f3f3f3f) cout << "I have no idea how to solve it.";
	else cout << dis[T];
	return 0;
}
