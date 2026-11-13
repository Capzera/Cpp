#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 1;
vector<int> g[N];
int n, m, u, v, ans, mn = INT_MAX, dis[N];
int bfs(int u) {
	memset(dis, 0x3f, sizeof(dis));
	int mx = 0;
	queue<int> q;
	dis[u] = 0;
	q.push(u);
	while (q.size()) {
		int u = q.front(); q.pop();
		for (auto& v : g[u]) {
			if (u == v) continue;
			if (dis[u] + 1 < dis[v]) {
				dis[v] = dis[u] + 1;
				mx = max(mx, dis[v]);
				q.push(v);
			}
		}
	}
	return mx;
}
int main() {
	cin >> n >> m;
	while (m--) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		int cur = bfs(u);
		if (cur < mn) {
			mn = cur;
			ans = u;
		}
	}
	cout << ans;
	return 0;
}
