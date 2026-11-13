#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, u, v, ans, f[N];
vector<int> g[N];
int dfs(int u, int fa) {
	int ans = 1;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		ans += dfs(v, u);
	}
	f[u] = ans;
	return f[u];
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0);
	for (int i = 1; i <= n; i++) cout << f[i] << " ";
	return 0;
}
