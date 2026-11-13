#include <bits/stdc++.h>
using namespace std;
const int N = 1.6e4 + 10;
int n, u, v, a[N], f[N], ans = INT_MIN;
vector<int> g[N];
void dfs(int u, int fa) {
	f[u] = a[u];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		if (f[v] > 0) f[u] += f[v];
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, -1);
	cout << *max_element(f + 1, f + n + 1);
	return 0;
}
