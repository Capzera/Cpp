#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 1e5 + 10;
vector<int> g[N];
int n, u, ans, vis[N], c[N];
int dfs(int u) {
	int ans = 0;
	for (auto& v : g[u]) ans += dfs(v);
	if (!ans) return c[u];
	return min(ans, c[u]);
}
signed main() {
	cin >> n;
	for (int v = 2; v <= n; v++) {
		cin >> u;
		g[u].push_back(v);
	}
	for (int i = 1; i <= n; i++) cin >> c[i];
	cout << dfs(1);
	return 0;
}

