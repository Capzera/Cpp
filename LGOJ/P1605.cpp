#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
int n, m, t, ans, x, y, sx, sy, fx, fy, a[6][6], vis[6][6];
void dfs(int x, int y) {
	if (x == fx && y == fy) {
		ans++;
		return;
	}
	for (int d = 0; d < 4; d++) {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 1 || mx > n || my < 1 || my > m || a[mx][my] || vis[mx][my]) continue;
		vis[mx][my] = 1;
		dfs(mx, my);
		vis[mx][my] = 0;
	}
}
int main() {
	cin >> n >> m >> t >> sx >> sy >> fx >> fy;
	while (t--) {
		cin >> x >> y;
		a[x][y] = 1;
	}
	vis[sx][sy] = 1;
	dfs(sx, sy);
	cout << ans;
	return 0;
}
