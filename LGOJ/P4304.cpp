#include <bits/stdc++.h>
using namespace std;
const int N = 205, M = 4e4 + 1;
const int dx[] = {-2, -1, -1, -2, 1, 2, 2, 1}, dy[] = {1, 2, -2, -1, -2, -1, 1, 2};
char a[N][N];
int n, m, u, v, k, ans, vis[M], match[M];
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
	cin >> n;
	ans = n * n;
	for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
		cin >> a[i][j];
		if (a[i][j] == '1') ans--;
	}
	for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
		if ((i + j) % 2 || a[i][j] == '1') continue;
		for (int d = 0; d < 8; d++) {
			int nx = i + dx[d], ny = j + dy[d];
			if (nx < 1 || nx > n || ny < 1 || ny > n || a[nx][ny] == '1') continue;
			add((i - 1) * n + j, (nx - 1) * n + ny);
		}
	}
	for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
		if ((i + j) % 2 || a[i][j] == '1') continue;
		k++;
		ans -= dfs((i - 1) * n + j);
	}
	cout << ans;
	return 0;
}
