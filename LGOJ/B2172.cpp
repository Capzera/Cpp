#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
vector<int> g[N];
int n, u, v, vis[N];
int main() {
	cin >> n;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int u = 1; u <= n; u++) sort(g[u].begin(), g[u].end());
	queue<int> q;
	q.push(1), vis[1] = 1;
	while (q.size()) {
		int u = q.front(); q.pop();
		cout << u << ' ';
		for (auto& v : g[u]) {
			if (vis[v]) continue;
			vis[v] = 1;
			q.push(v);
		}
	}
	return 0;
}
