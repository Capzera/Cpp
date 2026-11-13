#include <bits/stdc++.h>
using namespace std;
const int N = 6e3 + 10;
vector<int> g[N];
int n, u, v, root, a[N], dp1[N], dp2[N], vis[N];
void dfs(int u) {
	dp2[u] = 0;
	dp1[u] = a[u];
	for (auto& v : g[u]) {
		dfs(v);
		dp1[u] += dp2[v];
		dp2[u] += max(dp1[v], dp2[v]);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[v].push_back(u);
		vis[u] = 1;
	}
	for (int i = 1; i <= n; i++) {
		if (!vis[i]) root = i;
	}
	dfs(root);
	cout << max(dp1[root], dp2[root]);
	return 0;
}
