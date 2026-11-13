#include <bits/stdc++.h>
using namespace std;
const int N = 205;
int n, m, p, u, v, k, ans, a[N][N], vis[N], match[N];
vector<int> g[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> p;
	while (p--) {
		cin >> u >> v;
		a[u][v] = 1;
	}
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
		if (a[i][j]) continue;
		g[i].push_back(j);
	}
	for (int i = 1; i <= n; i++)  {
		k++;
		ans += dfs(i);
	}
	cout << ans;
	return 0;
}
