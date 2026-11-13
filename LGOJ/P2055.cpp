#include <bits/stdc++.h>
using namespace std;
const int N = 51;
int T, n, x, h[N], s[N], vis[N], match[N];
vector<int> g[N];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v]) continue;
		vis[v] = 1;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int solve() {
	cin >> n;
	int cnt = 0, ans = 0;
	for (int i = 1; i <= n; i++) cin >> s[i];
	for (int i = 1; i <= n; i++) {
		g[i].clear();
		cin >> h[i];
		if (s[i] && !h[i]) {
			g[i].push_back(i);
			cnt++;
		} else if (!s[i]) cnt++;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> x;
			if (x && s[j]) g[i].push_back(j);
		}
	}
	memset(match, 0, sizeof match);
	for (int u = 1; u <= n; u++) {
		if (!s[u] || !h[u]) {
			memset(vis, 0, sizeof vis);
			ans += dfs(u);
		}
	}
	return ans == cnt;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> T;
	while (T--) cout << (solve() ? "^_^\n" : "T_T\n");
	return 0;
}
