#include <bits/stdc++.h>
using namespace std;
int N, M, n = 1, m = 1, u, v, len;
char ans[769][1536];
bool vis[11][1025];
void dfs(int x, int y, int color, int l, int r, int dep, int p) {
	if (x == n) {
		ans[x][y] = 'o';
		return;
	}
	if (color == 1) {
		ans[x][y] = 'o';
		if (!vis[dep + 1][2 * p - 1]) dfs(x + 1, y - 1, 2, l + 1, r, dep + 1, 2 * p - 1);
		if (!vis[dep + 1][2 * p]) dfs(x + 1, y + 1, 3, l + 1, r, dep + 1, 2 * p);
	}
	if (color == 2) {
		ans[x][y] = '/';
		if (l * 2 == r) dfs(x + 1, y - 1, 1, 1, l, dep, p);
		else dfs(x + 1, y - 1, 2, l + 1, r, dep, p);
	}
	if (color == 3) {
		ans[x][y] = '\\';
		if (l * 2 == r) dfs(x + 1, y + 1, 1, 1, l, dep, p);
		else dfs(x + 1, y + 1, 3, l + 1, r, dep, p);
	}
}
int main() {
	cin >> N >> M;
	if (N == 1) return puts("o"), 0;
	while (M--) {
		cin >> u >> v;
		vis[u][v] = 1;
	}
	n = 3 * (1 << N - 2);
	m = 6 * (1 << N - 2) - 1;
	memset(ans, 32, sizeof(ans));
	dfs(1, m + 1 >> 1, 1, 1, n, 1, 1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) putchar(ans[i][j]);
		puts("");
	}
	return 0;
}
