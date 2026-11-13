#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e4 + 5;
int n, m, k, u, v, w, a[N], dis[N];
vector<pii> g[N];
bool check(int x) {
	memset(dis, 0x3f, sizeof dis);
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	pq.push({0, 1});
	dis[1] = 0;
	while (pq.size()) {
		auto[d, u] = pq.top(); pq.pop();
		if (d > dis[u]) continue;
		for (auto[v, w] : g[u]) {
			if (a[v] > x) continue;
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				pq.push({dis[v], v});
			}
		}
	}
	return dis[n] != 0x3f3f3f3f && dis[n] <= k; 
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> k;
	int l = 0, r = 0, ans = -1;
	for (int i = 1; i <= n; i++) cin >> a[i], r = max(r, a[i] + 1);
	l = max(a[1], a[n]);
	while (m--) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	while (l < r) {
		int mid = l + (r - l >> 1);
		if (check(mid)) {
			ans = r = mid;
		} else l = mid + 1;
	}
	if (ans == -1) return puts("AFK"), 0;
	cout << ans;
	return 0;
}
