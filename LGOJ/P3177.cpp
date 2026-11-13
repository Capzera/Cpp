#include <bits/stdc++.h>
#define pii pair<int, long long>
using namespace std;
const int N = 2e3 + 10;
int n, k, u, v, w, sz[N];
long long dp[N][N];
vector<pii> g[N];
int dfs(int u, int fa) {
	sz[u] = 1;
	dp[u][0] = 0, dp[u][1] = 0; // 0 白 1 黑
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		int sv = dfs(v, u);
		for (int i = min(k, sz[u] + sv); ~i; i--) {
			for (int j = max(0, i - sz[u]); j <= min(i, sv); j++) {
				if (dp[u][i - j] < 0) continue;
				long long bl = j * (k - j);
				long long wh = (sz[v] - j) * (n - k - sz[v] + j);
				dp[u][i] = max(dp[u][i], dp[u][i - j] + dp[v][j] + w * (bl + wh));
			}
		}
		sz[u] += sv;
	}
	return sz[u];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	memset(dp, -0x3f, sizeof dp);
	dfs(1, 0);
	cout << dp[1][k];
	return 0;
}
//dp[u][i] = max(dp[u][i], dp[u][i-j] + dp[v][j] + Cost(j))/
//Cost = w * [ j(k-j) + (sz[v]-j)(n-k-sz[v]+j) ]
