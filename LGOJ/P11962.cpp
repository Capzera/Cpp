#include <bits/stdc++.h>
using namespace std;
int n, u, v, dep[200001], ans[2];
vector<int> g[200001];
void dfs(int u, int fa) {
	dep[u] = dep[fa] + 1;
	ans[dep[u] & 1] ++;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	for (int i = 1; i <= n; i++) {
		cout << ans[dep[i] & 1] << " ";
	}
	
	return 0;
}
