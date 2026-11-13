#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int t, u, v, n, tot, a[N];
vector<int> g[N];
int dfs(int u, int fa) {
	int ans = 0, cnt = 0;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		int child = dfs(v, u);
		if (child == -1) return -1;
		if (child) {
			cnt++;
			ans += child;
		}
	}
	ans += a[u];
	if (cnt > 1 && ans != tot || cnt > 2) return -1;
	return ans;
}
int solve() {
	cin >> n;
	tot = 0;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		g[i].clear();
		tot += a[i];
	}
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	return dfs(1, 0);
}
int main() {
	cin >> t;
	while (t--) cout << (solve() == -1 ? "No\n" : "Yes\n");
	return 0;
}
