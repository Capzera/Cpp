#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;

long long t, n, m, K, p, u, v, w, f, ans;
long long vis[100010][51], dis[100010], dp[100001][51];
vector<vector<pii> > g1, g2;

void Dijkstra(int s) {
	vector<bool> vis(n + 1);
	memset(dis, 0x3f, sizeof(dis));
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	pq.push({0, s}), dis[s] = 0;
	while (pq.size()) {
		auto u = pq.top().second; pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto& [v, w] : g1[u]) {
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				pq.push({dis[v], v});
			}
		}
	}
}

long long dfs(int u, int k) {
	if (vis[u][k]) {
		f = 1;
		return 0;
	}
	if (dp[u][k] != -1) return dp[u][k];
	vis[u][k] = 1, dp[u][k] = 0;
	for (auto& [v, w] : g2[u]) {
		long long nk = dis[u] - dis[v] + k - w;
		if (nk < 0 || nk > K) continue;
		dp[u][k] = (dp[u][k] + dfs(v, nk)) % p;
		if (f) return 0;
	}
	vis[u][k] = 0;
	if (u == 1 && k == 0) dp[u][k] = 1;
	return dp[u][k];
}

int solve() {
	cin >> n >> m >> K >> p;
	g1 = g2 = vector<vector<pii> > (n + 1);
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> w;
		g1[u].push_back({v, w});
		g2[v].push_back({u, w});
	}
	Dijkstra(1);
	memset(dp, -1, sizeof(dp));
	memset(vis, 0, sizeof(vis));
	f = ans = 0;
	for (int i = 0; i <= K; i++) {
		ans = (ans + dfs(n, i)) % p;
		if (f) return -1;
	}
	return ans;
}

int main() {
	cin >> t;
	while (t--) {
		cout << solve() << endl;
	}
	return 0;
}
