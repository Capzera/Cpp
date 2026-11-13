#include<bits/stdc++.h>
using namespace std;
const int N = 22;
int n, k, u, v, sum, ans = INT_MAX, a[N];
vector<int> g[N];
bool vis[N][N];
void dfs(int idx, int sum, int cnt) {
	if (cnt >= ans) return;
	if (idx > n) {
		if (!sum) ans = min(ans, cnt);
		return;
	}
	dfs(idx + 1, sum, cnt);
	dfs(idx + 1, sum ^ a[idx], cnt + 1);
}
int main() {
	cin >> n;
	sum = (1ll << n) -1;
	for (int i = 1; i <= n; ++i) {
		cin >> k;
		for (int j = 1; j <= k; ++j) {
			cin >> u;
			if (i != u && !vis[i][u]) {
				g[i].push_back(u);
				vis[i][u] = 1;
			}
		}
	}
	for (int u = 1; u <= n; ++u) {
		a[u] ^= 1ll << u - 1;
		for (auto v : g[u]) {
			if (v == u) continue;
			a[u] ^= 1ll << v - 1;
			for (auto w : g[v]) {
				if (w == v) continue;
				a[u] ^= 1ll << w - 1;
			}
		}
	}
	dfs(1, sum, 0);
	if (ans == INT_MAX) {
		puts("Change an alarm clock，please!");
	} else cout << ans;
	return 0;
}
