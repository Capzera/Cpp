#include <bits/stdc++.h>
#define pii pair<int, long long>
using namespace std;
const int N = 1e5 + 10;
int n, u, v, w, mx, a[N], dis[N];
long long ans = LLONG_MAX, tot, dp[N], sum[N];
vector<pii> g[N];
int dfs(int u, int fa) {
	sum[u] = a[u];
	dp[1] += 1ll * a[u] * dis[u];
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dis[v] = dis[u] + w;
		dfs(v, u);
		sum[u] += sum[v];
	}
	return sum[u];
}
void dfs2(int u, int fa) {
	ans = min(ans, dp[u]);
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dp[v] = dp[u] + (sum[1] - 2 * sum[v]) * w;
		dfs2(v, u);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	dfs(1, 0);
	ans = dp[1];
	dfs2(1, 0);
	cout << ans;
	return 0;
}
