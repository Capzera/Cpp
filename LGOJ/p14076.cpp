#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
vector<pair<int, int> > g[N];
int n, m, u, v, w, cnt[N];
long long ans, tot;
void dfs(int u, int fa, long long sum) {
	ans = max(ans, sum);
	for (auto& [v, w] : g[u]) {
		if (v == fa) continue;
		dfs(v, u, sum + w);
	}
}
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
		tot += w * 2;
	}
	dfs(1, -1, 0);
	cout << tot - ans;
	return 0;
}
