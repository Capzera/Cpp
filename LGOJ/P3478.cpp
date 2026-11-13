#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, u, v, mx, ans = 1, dep[N], sz[N];
long long dp[N];
vector<int> g[N];
int dfs(int u, int fa) {
	sz[u] = 1;
	dp[1] += dep[u];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u);
		sz[u] += sz[v];
	}
	return sz[u];
}
void dfs2(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dp[v] = dp[u] - sz[v] + n - sz[v];
		dfs2(v, u);
	}
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	dfs2(1, 0);
	cout << max_element(dp + 1, dp + n + 1) - dp;
	return 0;
}
