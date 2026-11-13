#include <bits/stdc++.h>
using namespace std;
int n, u, v, ans, a[100001], cnt[100001];
vector<int> g[100001];

int dfs(int sn) {
	if (cnt[sn]) return cnt[sn];
	cnt[sn] = 1;
	for (auto& fn : g[sn]) {
		if (a[fn] < a[sn]) cnt[sn] += dfs(fn);
	}
	return cnt[sn];
}

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) {
		ans = max(ans, dfs(u));
	}
	cout << ans;
	return 0;
}
