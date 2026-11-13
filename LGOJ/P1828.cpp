#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 805, M = 2e3 + 10, P = 505;
int n, m, u, v, p, x, vis[N];
int w, ans = INT_MAX, dis[N];
vector<pii> g[N];
map<int, int> mp;
void Dijkstra(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
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
	cin >> p >> n >> m;
	while (p--) {
		cin >> x;
		mp[x]++;
	}
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	for (int i = 1; i <= n; i++) {
		Dijkstra(i);
		int sum = 0;
		for (auto&& [x, y] : mp) {
			if (dis[x] == 0x3f3f3f3f) continue;
			sum += dis[x] * y;
		}
		if (!sum) continue;
		ans = min(ans, sum);
	}
	cout << ans;
	return 0;
}
/*
3 5 5
2 3 4
1 2 1
1 3 5
2 3 7
2 4 3
3 4 5
*/
