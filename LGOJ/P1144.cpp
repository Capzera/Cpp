#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e6 + 10;
vector<vector<int> > edge;
int ans[maxn], vis[maxn], dis[maxn];
int n, m, u, v, mod = 100003;
void Dijkstra() {
	queue<pair<int, int> > q;
	q.push({0, 1});
	ans[1] = vis[1] = 1;
	while (q.size()) {
		auto[d, u] = q.front(); q.pop();
		for (auto& v : edge[u]) {
			if (!vis[v]) {
				vis[v] = 1;
				dis[v] = d + 1;
				ans[v] = (ans[v] + ans[u]) % mod;
				q.push({d + 1, v});
			} else if (d + 1 == dis[v]) {
				ans[v] = (ans[v] + ans[u]) % mod;
			}
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	edge.resize(n + 1);
	for (int i = 0; i < m; i++) {
		cin >> u >> v;
		if (u == v) continue;
		edge[u].emplace_back(v);
		edge[v].emplace_back(u);
	}
	Dijkstra();
	for (int i = 1; i <= n; i++) {
		cout << ans[i] % mod << endl;
	}
	return 0;
}
