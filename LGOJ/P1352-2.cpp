#include <bits/stdc++.h>
using namespace std;
const int N = 6e3 + 10;
vector<int> g[N];
int n, u, v, rt, a[N], dp[N][2], vis[N];
void dfs(int u) {
	dp[u][1] = a[u];
	for (auto& v : g[u]) {
		dfs(v);
		dp[u][1] += dp[v][0];
		dp[u][0] += max(dp[v][0], dp[v][1]);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[v].push_back(u);
		vis[u] = 1;
	}
	for (int i = 1; i <= n; i++) {
		if (!vis[i]) rt = i;
	}
	dfs(rt);
	cout << max(dp[rt][0], dp[rt][1]);
	return 0;
}
