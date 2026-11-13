#include <bits/stdc++.h>
using namespace std;
const int N = 205, M = 5005, INF = 0x3f3f3f3f;
struct Edge {
	int v, cap, rev;
};
int n, m, S, T, u, v, w, dep[N], cur[N];
long long ans;
vector<Edge> g[N];
bool bfs() {
	memset(dep, 0, sizeof dep);
	queue<int> q;
	q.push(S);
	dep[S] = 1;
	while (q.size()) {
		int u = q.front(); q.pop();
		for (auto& e : g[u]) {
			if (e.cap && !dep[e.v]) {
				dep[e.v] = dep[u] + 1;
				q.push(e.v);
			}
		}
	}
	return dep[T];
}
int dfs(int u, int flow) {
	if (u == T) return flow;
	int res = 0;
	for (int& i = cur[u]; i < g[u].size(); i++) {
		Edge& e = g[u][i];
		if (e.cap && dep[e.v] == dep[u] + 1) {
			int f = dfs(e.v, min(flow - res, e.cap));
			if (f) {
				e.cap -= f;
				g[e.v][e.rev].cap += f;
				res += f;
				if (res == flow) break;
			}
		}
	}
	return res;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> S >> T;
	while (m--) {
		cin >> u >> v >> w;
		g[u].push_back({v, w, g[v].size()});
		g[v].push_back({u, 0, g[u].size() - 1});
	}
	while (bfs()) {
		memset(cur, 0, sizeof cur);
		ans += dfs(S, INF);
	}
	cout << ans << '\n';
	return 0;
}
