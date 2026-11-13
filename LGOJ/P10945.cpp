#include <bits/stdc++.h>
using namespace std;
const int N = 55, M = 2505;
char a[N][N];
int T, n, m;
int row[N][N], col[N][N], match[M], vis[M];
vector<int> g[M];
int dfs(int u, int k) {
	for (auto& v : g[u]) {
		if (vis[v] == k) continue;
		vis[v] = k;
		if (!match[v] || dfs(match[v], k)) {
			match[v] = u;
			return 1;
		}
	}
	return 0;
}
int solve() {
	cin >> n >> m;
	int ans = 0, rcnt = 0, ccnt = 0, k = 0;
	memset(row, 0, sizeof row);
	memset(col, 0, sizeof col);
	memset(match, 0, sizeof match);
	memset(vis, 0, sizeof vis);
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
			if (a[i][j] == '#') continue;
			row[i][j] = rcnt;
			if (j == 1 || a[i][j - 1] == '#') row[i][j]++, rcnt++;
		}
	}
	for (int i = 1; i <= rcnt; i++) g[i].clear();
	for (int j = 1; j <= m; j++) {
		for (int i = 1; i <= n; i++) {
			if (a[i][j] == '#') continue;
			col[i][j] = ccnt;
			if (i == 1 || a[i - 1][j] == '#') col[i][j]++, ccnt++;
			if (a[i][j] == 'o') g[row[i][j]].push_back(col[i][j]);
		}
	}
	for (int u = 1; u <= rcnt; u++) {
		k++;
		ans += dfs(u, k);
	}
	return ans;
}
int main() {
	cin >> T;
	for (int i = 1; i <= T; i++) {
		printf("Case :%d\n%d\n", i, solve());
	}
	return 0;
}
