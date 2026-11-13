#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 3;
int n, u, v, ans, a[N];
vector<int> g[N];
int dfs(int u, int fa) {
	int chk = 0;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		if (dfs(v, u)) chk = 1;
	}
	if (!a[u] && chk) ans++;
	return a[u] || chk;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		if (!a[u]) continue;
		dfs(u, 0);
		break;
	}
	cout << ans;
	return 0;
}
