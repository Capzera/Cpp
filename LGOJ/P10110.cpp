#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
long long n, m, x, y, u, v, a[100001], dis[100010];
vector<int> g[100001];
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	vector<bool> vis(n);
	pq.push({0, s}); dis[s] = 0;
	while (pq.size()) {
		int u = pq.top().second; pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& v : g[u]) {
			if (dis[v] > dis[u] + 1) {
				dis[v] = dis[u] + 1;
				pq.push({dis[v], v});
			}
		}
	}
}
int main() {
	memset(dis, 0x3f, sizeof(dis));
	cin >> n >> m >> x >> y;
	for (int i = 0; i < n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
	}
	Dijkstra(x);
	if (dis[y] >= 0x3f3f3f3f / 2) return puts("No solution"), 0;
	cout << a[y] - a[x] + dis[y] << endl;
	return 0;
}
