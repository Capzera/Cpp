#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int t, n, u, v, dep[N], f[N];
vector<int> g[N];
int dfs(int u, int fa) {
	int ans = 1;
	for (auto& v : g[u]) { // 先孩子
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		ans += dfs(v, u);
	}
	f[u] = ans; // 后访问根
	return f[u];
}
void solve() {
	cin >> n;
	dep[1] = 1;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	for (int u = 1; u <= n; u++) {
		cout << dep[u] << " " << n - f[u] + 1 << '\n';
	}
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
