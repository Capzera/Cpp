#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 5;
vector<int> g[N];
int n, u1, u2, mx, k, vis[N], match[N];
bool dfs(int u) {
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
 	for (int v = 1; v <= n; v++) {
		cin >> u1 >> u2;
		g[u1].push_back(v);
		g[u2].push_back(v);
		mx = max({mx, u1, u2});
	}
	for (int u = 1; u <= mx; u++) {
		k++;
		if (!dfs(u)) return cout << u - 1, 0;
	}
	cout << mx;
	return 0;
}
