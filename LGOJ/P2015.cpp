#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e2 + 10;
int n, m, u, v, w, dp[N][N];
vector<pii> g[N];
void dfs(int u, int fa) {
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		for (int i = m; i >= 1; i--) {
			for (int j = 0; j < i; j++) {
				dp[u][i] = max(dp[u][i], dp[u][i - j - 1] + dp[v][j] + w);
			}
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	dfs(1, 0);
	cout << dp[1][m];
	return 0;
}
//dp[u][i] 表示u为根节点的子树上保留i条边能获得最多的苹果数量
//dp[u][i] = max(dp[u][i], dp[u][i - j - 1] + dp[v][j] + w(u, v));
