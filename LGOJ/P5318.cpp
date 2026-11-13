#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, ind[100001], vis[100001];
vector<int> g[100001];
queue<int> q;
int main() {
	cin >> n >> m;
	while (m--) {
		cin >> u >> v;
		g[u].push_back(v);
		ind[v]++;
	}
	for (int u = 1; u <= n; u++) {
		sort(g[u].begin(), g[u].end());
	}
	function<void(int)> dfs = [&](int sn) -> void {
		cout << sn << " ";
		for (auto& fn : g[sn]) {
			if (vis[fn]) continue;
			vis[fn] = 1;
			dfs(fn);
		}
	};
	function<void()> bfs = [&]() -> void {
		while (q.size()) {
			u = q.front(); q.pop();
			cout << u << " ";
			for (auto& v : g[u]) {
				if (vis[v]) continue;
				vis[v] = 1;
				q.push(v);
			}
		}
	};
	vis[1] = 1; dfs(1); cout << endl;
	memset(vis, 0, sizeof(vis));
	vis[1] = 1; q.push(1); bfs();

	return 0;
}
