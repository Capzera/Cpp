#include <bits/stdc++.h>
using namespace std;
int n, c, u, v, dep[100001];
vector<int> g[100001];
void dfs(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dep[v] = dep[u] + 1;
		if (dep[v] > dep[c]) c = v;
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
	dep[c] = 0;
	dfs(c, 0);
	cout << dep[c];
	return 0;
}
