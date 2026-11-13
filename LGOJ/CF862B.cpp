#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
vector<int> g[N];
long long n, u, v, bl, wt, vis[N];
void dfs(int u, int color) {
	vis[u] = color;
	color == 1 ? bl++ : wt++;
	for (auto& v : g[u]) {
		if (vis[v]) continue;
		dfs(v, 3 - color);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 1);
	cout << bl * wt - (n - 1);
	return 0;
}
