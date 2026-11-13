#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int n, m, u, v, vis[N];
vector<int> g[N];
queue<int> q;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		if (vis[u]) continue;
		vis[u] = 1;
		q.push(u);
		while (q.size()) {
			int u = q.front(); q.pop();
			for (auto& v : g[u]) {
				if (!vis[v]) {
					vis[v] = 3 - vis[u];
					q.push(v);
				} else if (vis[v] == vis[u]) return puts("No"), 0;
			}
		}
	}
	puts("Yes");
	return 0;
}
