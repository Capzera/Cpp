#include <bits/stdc++.h>
using namespace std;
const int N = 2E3 + 5;
vector<int> g[N];
int n, m, v1, v2, k, f, cnt, ans[N], vis[N], match[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			ans[u] = v;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int u = 1; u <= m; u++) {
		cin >> v1 >> v2;
		g[u].push_back(v1);
		g[u].push_back(v2);
	}
	for (int u = 1; u <= m; u++) {
		k++;
		if (!dfs(u)) break;
		cnt++;
	}
	cout << cnt << '\n';
	for (int u = 1; u <= cnt; u++) {
		cout << ans[u] << '\n';
	}
	return 0;
}
