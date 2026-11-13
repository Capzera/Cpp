#include <bits/stdc++.h>
using namespace std;
const int N = 2e3 + 10;
int n, u, v, k, dp[N][2];
vector<int> g[N];
void dfs(int u, int fa) {
	dp[u][1] = 1;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		dp[u][0] += dp[v][1];
		dp[u][1] += min(dp[v][0], dp[v][1]);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> u >> k;
		while (k--) {
			cin >> v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
	}
	dfs(0, -1);
	cout << min(dp[0][0], dp[0][1]);
	return 0;
}
