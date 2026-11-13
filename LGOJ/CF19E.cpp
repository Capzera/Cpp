#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
struct edge {
	int v, id;
};
vector<edge> g[N];
vector<int> ans;
int n, m, u, v, odd, ansid;
int vis[N], color[N], dep[N], diff[2][N], paid[N];

void dfs(int u, int pid) {
	vis[u] = 1;
	for (auto& [v, id] : g[u]) {
		if (id == pid) continue;
		if (vis[v]) {
			if (dep[u] <= dep[v]) continue;
			if (color[u] == color[v]) {
				odd++;
				diff[1][u]++, diff[1][v]--, ansid = id;
			} else {
				diff[0][u]++, diff[0][v]--;
			}
		} else {
			color[v] = color[u] ^ 1;
			dep[v] = dep[u] + 1;
			paid[v] = id;
			dfs(v, id);
			diff[1][u] += diff[1][v], diff[0][u] += diff[0][v];
		}
	}
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back({v, i});
		g[v].push_back({u, i});
	}
	for (int i = 1; i <= n; i++) {
		if (!vis[i]) dfs(i, 0);
	}
	if (!odd) {
		for (int i = 1; i <= m; i++) ans.push_back(i);
	} else {
		if (odd == 1) ans.push_back(ansid);
		for (int i = 1; i <= n; i++) {
			if (paid[i] && diff[1][i] == odd && !diff[0][i]) ans.push_back(paid[i]);
		}
	}
	sort(ans.begin(), ans.end());
	cout << ans.size() << '\n';
	for (auto& x : ans) cout << x << " ";
	return 0;
}
