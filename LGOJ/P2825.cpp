#include <bits/stdc++.h>
using namespace std;
const int N = 55, M = 2505;
char a[N][N];
int n, m, k, rcnt, ccnt, ans;
int row[N][N], col[N][N], match[M], vis[M];
vector<int> g[M];
int dfs(int u) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (!match[v] || dfs(match[v])) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == '#') continue;
			row[i][j] = rcnt;
			if (j == 1 || a[i][j - 1] == '#') row[i][j]++, rcnt++;
		}
	}
	for (int j = 1; j <= m; j++) {
		for (int i = 1; i <= n; i++) {
			if (a[i][j] == '#') continue;
			col[i][j] = ccnt;
			if (i == 1 || a[i - 1][j] == '#') col[i][j]++, ccnt++;
			if (a[i][j] == '*') g[row[i][j]].push_back(col[i][j]);
		}
	}
	for (int u = 1; u <= rcnt; u++) {
		k++;
		ans += dfs(u);
	}
	cout << ans;
	return 0;
}
