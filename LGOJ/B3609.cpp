#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 5;
int n, m, u, v;
int t, cnt, dfn[N], low[N], ins[N];
int top, stk[N];
vector<int> g[N];
vector<vector<int> > ans;
void Tarjan(int u) {
	dfn[u] = low[u] = ++t;
	stk[++top] = u;
	ins[u] = 1;
	for (auto& v : g[u]) {
		if (!dfn[v]) {
			Tarjan(v);
			low[u] = min(low[u], low[v]);
		} else if (ins[v]) {
			low[u] = min(low[u], dfn[v]);
		}
	}
	if (dfn[u] == low[u]) {
		vector<int> vec;
		do {
			v = stk[top--];
			vec.push_back(v);
			ins[v] = 0;
		} while (u != v);
		sort(vec.begin(), vec.end());
		ans.push_back(vec);
	}
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
	}
	for (int u = 1; u <= n; u++) {
		if (!dfn[u]) Tarjan(u);
	}
	cout << ans.size() << '\n';
	sort(ans.begin(), ans.end());
	for (auto& vec : ans) {
		for (auto& x : vec) cout << x << " ";
		cout << '\n';
	}
	return 0;
}


