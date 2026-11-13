#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10;
int n, u, v, dp[N][3];
vector<int> g[N];
void dfs(int u, int fa) {
	dp[u][0] = 1, dp[u][1] = INT_MAX;
	int sum = 0, cost = INT_MAX, f = 0;
	for (auto& v : g[u]) {
		if (v == fa) continue;
		f = 1;
		dfs(v, u);
		dp[u][0] += min({dp[v][0], dp[v][1], dp[v][2]});
		dp[u][2] += min(dp[v][0], dp[v][1]);
		
		sum += min(dp[v][0], dp[v][1]);
		cost = min(cost, dp[v][0] - min(dp[v][0], dp[v][1]));
	}
	if (f) dp[u][1] = sum + cost;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dp[0][0] = 0x3f3f3f3f;
	dfs(1, 0);
	cout << min(dp[1][0], dp[1][1]);
	return 0;
}
//dp[u][0/1/2] 表示点u为根节点的子树 0(建塔), 1(被儿子覆盖), 2(被父亲覆盖)的最小信号塔开销.
