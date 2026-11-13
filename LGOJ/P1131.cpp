#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 5e5 + 10;
long long n, s, u, v, w, ans, dp[N];
vector<pii> g[N];
void dfs(int u, int fa) {
	for (auto [v, w] : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		dp[u] = max(dp[u], dp[v] + w);
	}
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		ans += dp[u] - w - dp[v];
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> s;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	dfs(s, 0);
	cout << ans;
	return 0;
}
