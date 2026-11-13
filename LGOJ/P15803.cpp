#include <bits/stdc++.h>
using namespace std;
const int N = 5E3 + 10;
struct road {
	int u, v, w, b;
	void read() {
		cin >> u >> v >> w >> b;
	}
	bool operator < (const struct road& rhs) const {
		if (b != rhs.b) return b < rhs.b;
		return w < rhs.w;
	}
} e[N];
struct node {
	long long d, u;
	bool operator < (const node& rhs) const {
		return d > rhs.d;
	}
};
vector<road> g[N];
int n, m;
long long ans = LLONG_MAX, dis1[N], disn[N];
void Dijkstra(long long* dis, priority_queue<node>& pq) {
	while (pq.size()) {
		long long d = pq.top().d, u = pq.top().u; pq.pop();
		if (dis[u] < d) continue;
		for (auto& r : g[u]) {
			long long v = r.v, w = r.w;
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				pq.push({dis[v], v});
			}
		}
	}
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) e[i].read();
	sort(e + 1, e + m + 1);
	memset(dis1, 0x3f, sizeof(dis1));
	memset(disn, 0x3f, sizeof(disn));
	dis1[1] = disn[n] = 0;
	for (int i = 1, j; i <= m; i = j) {
		j = i;
		priority_queue<node> pq1, pqn;
		while (j <= m && e[j].b == e[i].b) {
			int u = e[j].u, v = e[j].v, w = e[j].w, b = e[j].b;
			g[u].push_back({u, v, w, b});
			g[v].push_back({v, u, w, b});
			pq1.push({dis1[u], u}), pq1.push({dis1[v], v});
			pqn.push({disn[u], u}), pqn.push({disn[v], v});
			j++;
		}
		Dijkstra(dis1, pq1), Dijkstra(disn, pqn);
		for (int k = i; k < j; k++) {
			int u = e[k].u, v = e[k].v;
			ans = min({ans, dis1[u] + disn[v], dis1[v] + disn[u]});
		}
		i = j;
	}
	cout << (ans == LLONG_MAX ? -1 : ans) << '\n';
	return 0;
}
