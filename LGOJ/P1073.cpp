#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
const int N = 1e5 + 5;
int n, m, u, v, s, ans, a[N], mn[N], mx[N];
vector<int> g1[N], g2[N];
void Dij1(int s) {
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	memset(mn, 0x3f, sizeof mn);
	mn[s] = a[s];
	pq.push({a[s], s});
	while (pq.size()) {
		auto [val, u] = pq.top(); pq.pop();
		if (mn[u] != val) continue;
		for (auto& v : g1[u]) {
			if (min(a[v], mn[u]) < mn[v]) {
				mn[v] = min(a[v], mn[u]);
				pq.push({mn[v], v});
			}
		}
	}
}
void Dij2(int s) {
	priority_queue<pii> pq;
	mx[s] = a[s];
	pq.push({a[s], s});
	while (pq.size()) {
		auto [val, u] = pq.top(); pq.pop();
		if (mx[u] != val) continue;
		for (auto& v : g2[u]) {
			if (max(a[v], mx[u]) > mx[v]) {
				mx[v] = max(a[v], mx[u]);
				pq.push({mx[v], v});
			}
		}
	}
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> s;
		g1[u].push_back(v);
		g2[v].push_back(u);
		if (s == 2) {
			g1[v].push_back(u);
			g2[u].push_back(v);
		}
	}
	Dij1(1), Dij2(n);
	for (int i = 1; i <= n; i++) {
		ans = max(ans, mx[i] - mn[i]);
	}
	cout << ans;
	return 0;
}
