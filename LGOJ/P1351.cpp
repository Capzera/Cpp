#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10, mod = 1e4 + 7;
vector<int> g[N];
long long n, u, v, mx, ans, w[N];

void dfs(int u, int fa) {
	long long sum1 = 0, sum2 = 0, mx1 = 0, mx2 = 0;
	for (auto& v : g[u]) {
		if (w[v] > mx1) {
			mx2 = mx1;
			mx1 = w[v];
		} else if (w[v] > mx2) mx2 = w[v];
		sum1 += w[v];
		sum2 += w[v] * w[v];
		if (v == fa) continue;
		dfs(v, u);
	}
	ans = (ans + sum1 * sum1 - sum2) % mod;
	mx = max(mx, mx1 * mx2);
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= n; i++) cin >> w[i];
	dfs(1, 0);
	cout << mx << " " << ans;
	return 0;
}


