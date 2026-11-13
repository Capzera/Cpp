#include <bits/stdc++.h>
using namespace std;
int dir[] = {1, 0, -1, 0, 1};
int n, sx, sy, ans, a[101][101], vis[101][101];
void dfs(int x, int y, int sum) {
	ans = max(ans, sum);
	for (int d = 0; d < 4; d++) {
		for (int len = 1; len <= n; len++) {
			int nx = x + dir[d] * len, ny = y + dir[d + 1] * len;
			if (nx < 1 || nx > n || ny < 1 || ny > n) break;
			if (a[nx][ny]) continue;
			if (len == 1 || vis[nx][ny]) break;
			vis[nx][ny] = 1;
			dfs(nx, ny, sum + abs(x - nx) + abs(y - ny));
			vis[nx][ny] = 0;
			break;
		}
	}
}
int main() {
	cin >> n >> sx >> sy;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> a[i][j];
	vis[sx][sy] = 1;
	dfs(sx, sy, 0);
	cout << ans;
	return 0;
}
