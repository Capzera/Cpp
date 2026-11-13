#include <bits/stdc++.h>
using namespace std;
int n, m, u, v, ans, a[100010], cnt[100010], dp[100010][11];
vector<int> g[100010];
queue<int> q;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		cnt[v]++;
	}
	for (int i = 1; i <= n; i++) {
		if (cnt[i]) continue;
		q.push(i);
		dp[i][a[i]] = 1;
	}
	while (q.size()) {
		int u = q.front(); q.pop();
		for (auto& i : g[u]) {
			for (int j = 1; j <= 10; j++) {
				dp[i][j] = max(dp[i][j], dp[u][j]);
			}
			if (--cnt[i] == 0) {
				for (int j = a[i]; j >= 1; j--) {
					dp[i][a[i]] = max(dp[i][a[i]], dp[i][j] + 1);
				}
				q.push(i);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= 10; j++) {
			ans = max(ans, dp[i][j]);
		}
	}
	cout << ans;
	return 0;
}
