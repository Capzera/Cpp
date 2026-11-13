#include <bits/stdc++.h>
#define pii pair<int, long long>
using namespace std;
const long long INF = 0x3f3f3f3f3f3f3f3f, N = 505, M = 5e5 + 5;
long long c, n, m, d, q, k, u, v, w, x, y, a[M];
long long dis[N][N];
vector<pii> g[M];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	memset(dis, 0x3f, sizeof dis);
	cin >> c >> n >> m >> d >> q;
	for (int i = 1; i <= m; i++) cin >> a[i];
	for (int v = 1; v <= n; v++) {
		dis[v][v] = 0;
		cin >> k;
		while (k--) {
			cin >> u >> w;
			g[u].push_back({v, w});
		}
	}
	for (int v = 1; v <= m; v++) {
		int sz = g[v].size();
		for (int i = 0; i < sz; i++) {
			for (int j = 0; j < sz; j++) {
				int l1 = g[v][i].first, l2 = g[v][j].first;
				long long cost = g[v][i].second + g[v][j].second;
				dis[l1][l2] = min(dis[l1][l2], cost);
			}
		}
	}
	for (int k = 1; k <= n; k++) {
		for (int i = 1; i <= n; i++) {
			if (dis[i][k] == INF) continue;
			for (int j = 1; j <= n; j++) {
				if (dis[k][j] == INF) continue;
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
			}
		}
	}
	while (q--) {
		cin >> u >> v;
		long long ans = INF;
		for (pii st : g[u]) {
			for (pii ed : g[v]) {
				ans = min(ans, st.second + dis[st.first][ed.first] + ed.second);
			}
		}
		cout << ans + a[u] + a[v] << '\n';
	}
	return 0;
}
