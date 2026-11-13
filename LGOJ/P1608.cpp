#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		if (u != rhs.u) return u < rhs.u;
		if (v != rhs.v) return v < rhs.v;
		return w < rhs.w;
	}
	bool operator == (const struct edge& rhs) const {
		return u == rhs.u && v == rhs.v;
	}
} e[4000000];
vector<pii> g[2010];
int ans[2010], vis[2010], dis[2010];
int n, m, u, v, w;
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > q;
	q.push({0, s});
	ans[s] = 1, dis[s] = 0;
	while (q.size()) {
		auto u = q.top().second; q.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& [v, w] : g[u]) {
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				ans[v] = ans[u];
				if (!vis[v]) q.push({dis[v], v});
			} else if (dis[u] + w == dis[v]) {
				ans[v] += ans[u];
			}
		}
	}
}
int main() {
	cin >> n >> m;
	memset(dis, 0x3f, sizeof(dis));
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u >> e[i].v >> e[i].w;
	}
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		if (e[i] == e[i - 1]) continue;
		g[e[i].u].push_back({e[i].v, e[i].w});
	}
	Dijkstra(1);
	if (dis[n] == 0x3f3f3f3f) return puts("No answer"), 0;
	cout << dis[n] << " " << ans[n];
	return 0;
}
