#include <bits/stdc++.h>
using namespace std;
int n, c, u, v, w, dep[200001];
long long sum[200001];
vector<pair<int, int> > g[200001];
void dfs(int u, int fa) {
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		if (dep[v] > dep[c]) c = v;
		sum[v] = sum[u] + 
		dfs(v, u);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	dfs(1, 0);
	dep[c] = 0;
	dfs(c, 0);
	if (dep[c] & 1) dep[c]--;
	cout << dep[c];
	return 0;
}
