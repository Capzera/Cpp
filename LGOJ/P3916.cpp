#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, ans[100001];
int main() {
	cin >> n >> m;
	vector<int> g[n + 1];
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[v].push_back(u);
	}
	function<void(int, int)> dfs = [&](int sn, int fa) -> void {
		if (ans[sn]) return;
		ans[sn] = fa;
		for (auto& fn : g[sn]) {
			if (fn == fa) continue;
			dfs(fn, fa);
		}
	};
	for (int i = n; i > 0; i--) dfs(i, i);
	for (int i = 1; i <= n; i++) cout << ans[i] << " ";
	return 0;
}
