#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 3e2 + 10;
int n, m, u, v, w, a[N], dp[N][N];
vector<int> g[N];
void dfs(int u) {
	dp[u][1] = a[u];
	for (auto& v : g[u]) {
		dfs(v);
		for (int i = m + 1; i; i--) {
			for (int j = 0; j < i; j++) {
				dp[u][i] = max(dp[u][i], dp[u][i - j] + dp[v][j]);
			}
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int v = 1; v <= n; v++) {
		cin >> u >> a[v];
		g[u].push_back(v);
	}
	dfs(0);
	cout << dp[0][m + 1];
	return 0;
}
