#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5;
int n, x, k, ans[N], vis[N], match[N];
vector<int> g[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int u = 0; u < n; u++) {
		cin >> x;
		int v1 = (u + x) % n, v2 = (u - x + n) % n;
		if (v1 > v2) swap(v1, v2);
		g[u].push_back(v1);
		if (v2 != v1) g[u].push_back(v2);
	}
	for (int u = n - 1; ~u; u--) {
		k++;
		if (!dfs(u)) return puts("No Answer"), 0;
	}
	for (int v = 0; v < n; v++) {
		ans[match[v]] = v;
	}
	for (int i = 0; i < n; i++) cout << ans[i] << " ";
	return 0;
}
