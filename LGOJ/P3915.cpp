#include <bits/stdc++.h>
using namespace std;
int t, n, k, u, v;
vector<int> g[100001];
int dfs(int u, int fa) {
	int ans = 1;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		int child = dfs(v, u);
		if (child == k) continue;
		if (child == -1 || child > k) return -1;
		ans += child;
	}
	return ans;
}
int solve() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	if (n % k) return 0;
	return dfs(1, -1) == k;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "YES\n" : "NO\n");
	return 0;
}
