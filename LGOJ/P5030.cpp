#include <bits/stdc++.h>
using namespace std;
const int N = 205, M = 4e4 + 1;
const int dx[] = {-3, -1, -1, -3, 1, 3, 3, 1}, dy[] = {1, 3, -3, -1, -3, -1, 1, 3};
int n, m, p, u, v, k, ans, a[N][N], vis[M], match[M];
int head[M], nxt[M * 8], to[M * 8], tot;
void add(int u, int v) {
	to[++tot] = v;
	nxt[tot] = head[u];
	head[u] = tot;
}
int dfs(int u) {
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
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
	cin >> n >> m >> p;
	ans = n * m;
	while (p--) {
		cin >> u >> v;
		if (!a[u][v]) {
			a[u][v] = 1;
			ans--;
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (i % 2 || a[i][j]) continue;
			for (int d = 0; d < 8; d++) {
				int nx = i + dx[d], ny = j + dy[d];
				if (nx < 1 || nx > n || ny < 1 || ny > m || a[nx][ny]) continue;
				add((i - 1) * m + j, (nx - 1) * m + ny);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (i % 2 || a[i][j]) continue;
			k++;
			ans -= dfs((i - 1) * m + j);
		}
	}
	cout << ans;
	return 0;
}
