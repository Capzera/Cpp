#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10, mod = 1e9 + 7;
vector<int> g[N];
int n, m, u, v, a[N], dp[N][4];
void dfs(int u, int fa) {
	if (a[u]) dp[u][a[u]] = 1;
	else dp[u][1] = dp[u][2] = dp[u][3] = 1;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		for (int c = 1; c <= 3; c++) {
			if (a[u] && c != a[u]) continue;
			long long sum = 0;
			for (int cc = 1; cc <= 3; cc++) {
				if (c == cc) continue;
				sum = (sum + dp[v][cc]) % mod;
			}
			dp[u][c] = (dp[u][c] * sum) % mod;
		}
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= m; i++) {
		cin >> u >> a[u];
	}
	dfs(1, 0);
	cout << (dp[1][1] + dp[1][2] + dp[1][3]) % mod;
	return 0;
}
