#include <bits/stdc++.h>
using namespace std;
const int N = 110;
long long n, m, u, v, flag, d[N], c[N], U[N];
long long g[N][N];
void dfs(long long u) {
	c[u] = -U[u];
	long long cnt = 0;
	for (int v = 1; v <= n; v++) {
		if (g[v][u] == INT_MIN) continue;
		if (c[v] == INT_MIN) dfs(v);
		if (c[v] > 0) c[u] += c[v] * g[v][u];
	}
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> c[i] >> U[i];
		if (!c[i]) c[i] = INT_MIN;
	}
	for (int i = 1; i <= m; i++) {
		cin >> u >> v >> g[u][v];
		d[u]++;
	}
	for (int u = 1; u <= n; u++) {
		if (c[u] == INT_MIN) dfs(u);
	}
	for (int i = 1; i <= n; i++) {
		if (!d[i] && c[i] > 0) {
			cout << i << " " << c[i] << '\n';
			flag = 1;
		}
	}
	if (!flag) cout << "NULL";
	return 0;
}
