#include <bits/stdc++.h>
using namespace std;
int n, c, u, v, k, dep[100001], a[100001];
long long ans;
vector<int> g[100001];
void dfs(int u, int fa) {
	if (dep[u] <= 2 * k) ans += a[u];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u);
	}
}
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	cout << ans;
	return 0;
}
