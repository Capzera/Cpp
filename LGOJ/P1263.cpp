#include <bits/stdc++.h>
using namespace std;
const int N = 205, M = 4e4 + 5;
int n, m, k, rcnt, ccnt, ans;
int a[N][N], row[N][N], col[N][N], match[M], vis[M];
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
			if (a[i][j] == 2) continue;
			row[i][j] = rcnt;
			if (j == 1 || a[i][j - 1] == 2) row[i][j]++, rcnt++;
		}
	}
	for (int j = 1; j <= m; j++) {
		for (int i = 1; i <= n; i++) {
			if (a[i][j] == 2) continue;
			col[i][j] = ccnt;
			if (i == 1 || a[i - 1][j] == 2) col[i][j]++, ccnt++;
			if (!a[i][j]) g[row[i][j]].push_back(col[i][j]);
		}
	}
	for (int u = 1; u <= rcnt; u++) {
		k++;
		ans += dfs(u);
	}
	cout << ans << '\n';
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j]) continue;
			if (match[col[i][j]] == row[i][j]) {
				cout << i << " " << j << '\n';
			}
		}
	}
	return 0;
}
