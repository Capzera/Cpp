#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 1, K = 21;
int n, k, u, v, a[N];
long long dp[N][K];
vector<int> g[N];
void dfs1(int u, int fa) {
	dp[u][0] = a[u];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs1(v, u);
		for (int i = 1; i <= k; i++) dp[u][i] += dp[v][i - 1];
	}
}
void dfs2(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		for (int i = k; i >= 2; i--) {
			dp[v][i] += dp[u][i - 1] - dp[v][i - 2];
		}
		dp[v][1] += dp[u][0];
		dfs2(v, u);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= n; i++) cin >> a[i];
	dfs1(1, 0);
	dfs2(1, 0);
	for (int i = 1; i <= n; i++) {
		cout << accumulate(dp[i], dp[i] + k + 1, 0ll) << '\n';
	}
	return 0;
}
