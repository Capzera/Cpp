#include <bits/stdc++.h>
using namespace std;
int n, k, u, v, ans, a[1001];
vector<int> g[1001];
void dfs(int u, int fa, int white, int black) {
	if (black > k) return;
	ans = max(ans, white + black);
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u, white + 1 - a[v], black + a[v]);
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
	for (int i = 1; i <= n; i++) {
		dfs(i, 0, !a[i], a[i]);
	}
	cout << ans;
	return 0;
}
