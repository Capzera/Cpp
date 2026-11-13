#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5, M = 1e6 + 5;
int n, m, p, s, x, u, v, ans, w[N], a[N], vis[N];
vector<int> g[N];
bool cmp(int x, int y) {
	return w[x] > w[y];
}
void dfs(int u, int fa, int val) {
	vis[u] = 1;
	for (auto& v : g[u]) {
		if (vis[v] || v == fa || v == s || w[v] > val) continue;
		w[v] = val;
		dfs(v, u, val);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> p >> s;
	for (int i = 1; i <= n; i++) cin >> w[i];
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= p; i++) cin >> a[i];
	sort(a + 1, a + n + 1, cmp);
	for (int i = 1; i <= p; i++) dfs(a[i], 0, w[a[i]]);
	for (auto& v : g[s]) {
		ans += vis[v];
	}
	cout << ans;
	return 0;
}
