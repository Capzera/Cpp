#include <bits/stdc++.h>
using namespace std;
const int N = 20;
const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n, m, k, ans, sx, sy, fx, fy, x, y, vis[N][N];
void dfs(int x, int y) {
	if (x == fx && y == fy) {
		ans++;
		return;
	}
	for (int d = 0; d < 4; d++) {
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > m || vis[nx][ny]) continue;
		vis[nx][ny] = 1;
		dfs(nx, ny);
		vis[nx][ny] = 0;
	}
}
int main() {
	cin >> n >> m >> k >> sx >> sy >> fx >> fy;
	while (k--) {
		cin >> x >> y;
		vis[x][y] = 1;
	}
	vis[sx][sy] = 1;
	dfs(sx, sy);
	cout << ans;
	return 0;
}
