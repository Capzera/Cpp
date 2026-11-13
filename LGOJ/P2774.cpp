#include <bits/stdc++.h>
using namespace std;
const int N = 101, M = 1e4 + 1;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, -1, 0, 1};
int n, m, u, v, k, ans, a[N][N], vis[M], match[M];
int head[M], nxt[M * 4], to[M * 4], tot;
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
	cin >> m >> n;
	ans = n * m;
	for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
		cin >> a[i][j];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if ((i + j) % 2 || a[i][j]) continue;
			for (int d = 0; d < 4; d++) {
				int nx = i + dx[d], ny = j + dy[d];
				if (nx < 1 || nx > n || ny < 1 || ny > m || a[nx][ny]) continue;
				add((i - 1) * m + j, (nx - 1) * m + ny);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if ((i + j) % 2 || a[i][j]) continue;
			k++;
			ans -= dfs((i - 1) * n + j);
		}
	}
	cout << ans;
	return 0;
}
