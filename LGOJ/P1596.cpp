#include <bits/stdc++.h>
using namespace std;
const int dx[] = {1, -1, 0, 0, 1, 1, -1, -1}, dy[] = {0, 0, 1, -1, -1, 1, -1, 1};
char a[101][101];
int n, m, ans;
void dfs(int x, int y) {
	a[x][y] = '.';
	for (int d = 0; d < 8; d++)  {
		int mx = x + dx[d], my = y + dy[d];
		if (mx < 1 || mx > n || my < 1 || my > m || a[mx][my] != 'W') continue;
		dfs(mx, my);
	}
}
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i][j] == '.') continue;
			dfs(i, j);
			ans++;
		}
	}
	cout << ans;
	return 0;
}
