#include <bits/stdc++.h>
using namespace std;
const int dx[] = {0, 1, 0, -1, 1, 1, -1, -1};
const int dy[] = {1, 0, -1, 0, -1, 1, -1, 1};
int t, n, m, ans, sum, a[10][10], vis[10][10];
void dfs(int x, int y) {
	if (y > m) {dfs(x + 1, 1); return;}
	if (x > n) {ans = max(ans, sum); return;}
	dfs(x, y + 1);
	if (vis[x][y]) return; 
	sum += a[x][y];
	for (int d = 0; d < 8; d++) {
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
		vis[nx][ny]++;
	}
	dfs(x, y + 1);
	for (int d = 0; d < 8; d++) {
		int nx = x + dx[d], ny = y + dy[d];
		if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
		vis[nx][ny]--;
	}
	sum -= a[x][y];
}
long long solve() {
	ans = sum = 0;
	memset(vis, 0, sizeof(vis));
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	dfs(1, 1);
	return ans;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
