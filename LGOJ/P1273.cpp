#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 3e3 + 10;
int n, m, k, u, v, w, sz[N], dp[N][N];
vector<pii> g[N];
int dfs(int u) {
	if (u > n - m) return sz[u] = 1;
	for (auto [v, w] : g[u]) {
		sz[u] += dfs(v);
		for (int i = sz[u]; i; i--) {
			for (int j = 1; j <= min(i, sz[v]); j++) {
				dp[u][i] = max(dp[u][i], dp[u][i - j] + dp[v][j] - w);
			}
		}
	}
	return sz[u];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	memset(dp, -0x3f, sizeof dp);
	cin >> n >> m;
	for (int u = 1; u <= n - m; u++) {
		cin >> k;
		while (k--) {
			cin >> v >> w;
			g[u].push_back({v, w});
		}
	}
	for (int i = 1; i <= n; i++) dp[i][0] = 0;
	for (int i = n - m + 1; i <= n; i++) cin >> dp[i][1];
	dfs(1);
	for (int i = m; i; i--) {
		if (dp[1][i] >= 0) {
			cout << i;
			return 0;
		}
	}
	return 0;
}

// dp[u][i] 表示以u为根的子树中，满足i个用户观看比赛，所能获得的最大利润
